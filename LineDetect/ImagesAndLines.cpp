/**
 * @brief	La función detecta las líneas del asfalto
 * @arg		Se le pasa por argumento un puntero a la frame a analizar y un puntero al lugar donde se guardarán las lineas
 * @return	La función devuelve la ubicación de las lineas detecatadas, mediante un puntero a la estructura "Linea"
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	24/07/2014
 **/

#include "Header.h"

Linea Struct_Linea ( float rho, float theta, int y1, int y2)
{
  Linea ret;

  ret.rho = rho;
  ret.theta = theta;

  double a = cos(theta);
  double b = sin(theta);

  ret.punto1.x = (rho -(y1* b))/a;
  ret.punto1.y = y1;
  ret.punto2.x = (rho-(y2*b))/a;
  ret.punto2.y = y2;

  return ret;
}

/**
 * @brief	La función detecta las líneas del asfalto
 * @arg		Se le pasa por argumento un puntero a la frame a analizar y vector donde se guardarán las lineas.
 * @param img_rgb  Frame a analizar
 * @param lineas Vector donde se guardaran las lineas detectadas.
 * @return	La función devuelve por referencia la ubicación de las lineas detecatadas (3 lineas), almacenandolas en un vector de estructura "Linea" con memoria suficiente
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	24/07/2014
 **/

void ImagesAndLines (IplImage **img_rgb, Linea *lineas) {

  int i=0;
  unsigned int width, height;

  IplImage *img_gray = NULL;
  IplImage *img_back = NULL;
  IplImage *img_bw = NULL;
  IplImage *img_bordes = NULL;
  IplConvKernel *SE_disk = NULL;
  IplConvKernel *SE_disk2 = NULL;
  CvSize img_size;


  CvMemStorage* storage = cvCreateMemStorage(0);
  float rho;
  float theta;
  CvSeq* lines = NULL;
  CvSeqBlock * Block;
  struct { float rho_sum;float theta_sum;int cnt; } aux_lines[3];
  for (i=0;i<3;i++)
  {
    aux_lines[i].cnt = 0;
    aux_lines[i].rho_sum = 0;
    aux_lines[i].theta_sum =0;
  }

  width = (*img_rgb)->width;
  height = (*img_rgb)->height;
  img_size = cvSize(width,height);
  img_gray =  cvCreateImage(img_size, (*img_rgb)->depth, 1);
  img_back =  cvCreateImage(img_size, (*img_rgb)->depth, 1);
  img_bw =  cvCreateImage(img_size, (*img_rgb)->depth, 1);
  img_bordes =  cvCreateImage(img_size, (*img_rgb)->depth, 1);

  // Muestro salida.
   //cvNamedWindow("DEBUG_1", CV_WINDOW_AUTOSIZE);
   //cvNamedWindow("DEBUG_2", CV_WINDOW_AUTOSIZE);

   // RGB2GRAY
  cvCvtColor(*img_rgb, img_gray, CV_RGB2GRAY);

  // SDL: Genera elemento morfológico tipo cruz 20 x 20 ¿pero con centro en (1,1)?.
  // Imopen
  SE_disk = cvCreateStructuringElementEx(SE_DISCK, SE_DISCK, ANCHOR_X, ANCHOR_Y, CV_SHAPE_CROSS, NULL);
  // SE_disk = cvCreateStructuringElementEx(SE_DISCK, SE_DISCK, 5, 5, CV_SHAPE_ELLIPSE, NULL);

  // SDL: Morph open: hace erode y luego dilate -> Elimina detalle. Iteración: 1.
  cvMorphologyEx(img_gray, img_back, NULL, SE_disk, CV_MOP_OPEN, 1);

  // Salida intermedia.
  //cvShowImage( "DEBUG_1", img_back );

  // I = I - background
  // cvAbsDiff(img_gray, img_back, img_gray);
  cvSub(img_gray, img_back, img_gray, NULL);

  // Salida intermedia.
  //cvShowImage( "DEBUG_1", img_gray );

  // Graythresh / gray2bw	// Matlab usa el método de Otsu
  cvThreshold(img_gray, img_bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  // cvThreshold(img_gray, img_bw, 40, 255, CV_THRESH_BINARY);

  // Salida intermedia.
  //cvShowImage( "DEBUG_2", img_bw );

  // Imopen
  SE_disk2 = cvCreateStructuringElementEx(SE_DISCK_2, SE_DISCK_2, ANCHOR_X, ANCHOR_Y, CV_SHAPE_CROSS, NULL);
  cvMorphologyEx(img_bw, img_bw, NULL, SE_disk2, CV_MOP_OPEN, 1);

  // Salida intermedia.
  //cvShowImage( "DEBUG_1", img_bw );

  // Sobel vertical
  cvCanny(img_bw, img_bordes, 40, 200, 3);
  // cvSobel(img_bw, img_bordes, XORDER, YORDER, KERNEL);

  // Salida intermedia.
  //cvShowImage( "DEBUG_2", img_bordes );

  // SDL: En la imagen procesada por Canny, busca líneas.
  lines = cvHoughLines2( img_bordes, storage, CV_HOUGH_STANDARD, rho_res, theta_res, threshold, min_length, max_gap );

  // SDL: Acumula las líneas encontradas en 3 líneas: IZQ, CENTRO y DERECHA.
  //

  Block = lines->first;

  if (lines->total)
  {
    for (i=0; i<lines->total;i++)
    {
      theta = ((float*)(Block->data))[1+i*2];
      if (theta<1.2 || theta>2)
      {
	rho=((float*)(Block->data))[0+i*2];

	// Acumula lineas centro, izq y derecha.
	if (COND_CENTRO)
	{
	  if (rho < 0)
	  {
	    rho = -rho;
	    theta = theta - CV_PI;
	  }
	  aux_lines[1].rho_sum += rho;
	  aux_lines[1].theta_sum +=theta;
	  aux_lines[1].cnt += 1;

	}

	if (COND_IZQ)
	{
	  aux_lines[0].rho_sum +=rho;
	  aux_lines[0].theta_sum +=theta;
	  aux_lines[0].cnt +=1;
	}

	if (COND_DER)
	{
	  aux_lines[2].rho_sum +=rho;
	  aux_lines[2].theta_sum +=theta;
	  aux_lines[2].cnt +=1;
	}
      }
    }
  }

  // Promedia las líneas (angulo y rho).
  if (aux_lines[0].cnt)
  {
    rho = aux_lines[0].rho_sum/aux_lines[0].cnt;
    theta = aux_lines[0].theta_sum /aux_lines[0].cnt;
    lineas[0] = Struct_Linea(rho,theta,1,height-1);
  }
  else
  {
    lineas[0] = Struct_Linea(0,0,0,0);
  }


  if (aux_lines[1].cnt)
  {
    rho = aux_lines[1].rho_sum/aux_lines[1].cnt;
    theta = aux_lines[1].theta_sum /aux_lines[1].cnt;

    if (theta <0)
    {
      rho = -rho;
      theta = theta + CV_PI;
    }

    lineas[1] = Struct_Linea(rho,theta,1,height-1);
  }
  else
  {
    lineas[1] = Struct_Linea(0,0,0,0);
  }


  if (aux_lines[2].cnt)
  {
    rho = aux_lines[2].rho_sum/aux_lines[2].cnt;
    theta = aux_lines[2].theta_sum /aux_lines[2].cnt;
    lineas[2] = Struct_Linea(rho,theta,1,height-1);
  }
  else
  {
    lineas[2] = Struct_Linea(0,0,0,0);
  }

  Filtro_Lineas (img_bw, lineas, lineas);

  cvReleaseImage(&img_gray);
  cvReleaseImage(&img_back);
  cvReleaseImage(&img_bw);
  cvReleaseImage(&img_bordes);
  cvReleaseStructuringElement(&SE_disk);
  cvReleaseStructuringElement(&SE_disk2);
  cvClearMemStorage(storage);
  cvReleaseMemStorage(&storage);

  return;
}



