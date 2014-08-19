/**
 * \file	Main.c
 * \brief	Main program file
 * \c		Compilar: make
 * \arg		Por linea de comandos se indica si se analiza video por camara o video.  Se debe indicar archivo de video a analizar
 * \author 	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * \date 	31/08/2014
 **/

#include "Header.h"
#include <opencv2/imgproc/imgproc.hpp>


CvCapture* capture = NULL;
IplImage *croped = NULL;

uint8_t
LineDetectInit( int argc, char *argv[] )
{
	// Declaro variables
	char *filename = NULL;


	int fps;

	// Check vargins
	if (argc < 4)
		help();
	else{
	    if(strcmp(argv[1],"-v") == 0){
	    	filename = argv[2];
	    	capture = cvCaptureFromFile(filename);

	    	if ( !capture )
	    	{
	    		fprintf (stderr,"No se pudo abrir el video\n" );
	    		return 1;
	    	}
	    	//frame = cvQueryFrame( capture );
	    	cvQueryFrame( capture );

	    	// Get the fps, needed to set the delay
	    	fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );
	      }
	      else if (strcmp(argv[1],"-c") == 0){

	      //capture = cvCaptureFromCAM( CV_CAP_ANY );
	      capture = cvCaptureFromCAM( atoi(argv[2]) );
	      //fps = atoi(argv[2]);
	      if ( !capture ) {
	    	  fprintf( stderr, "ERROR al abrir camara frontal\n" );
	    	  return 1;
	      }

	      }else{
	    	printf("\n Parametros invalidos\n");
	    	help();
	    	return 1;
	      }
	  }

	  // Create a window to display the video

	  //cvNamedWindow("video", CV_WINDOW_AUTOSIZE);

	  printf("Inicializa OK\n");
	  return 0;
}

void
LineDetectHandler(IplImage** draw)
{
	IplImage* frame = NULL;
	Linea lineas[3];
	int k = 0,i;
	int * abs = NULL;
    char array[20];
    CvFont font;

	// get the image frame
	frame = cvQueryFrame( capture );
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 2, 8 );

	if( frame )
	{
		//-------------------------------------------------------------------------------------------------
		// Las lineas y llamadas que están entre estas lineas corresponden al soft de detección de lineas
		//-------------------------------------------------------------------------------------------------

		//Recorte del Frame para obtener la ROI

		croped = CropImage (frame,CROP_X,CROP_Y,CROP_WIDTH,CROP_HEIGHT);

		//Obtiene lineas utilizando transformada de Hough

		ImagesAndLines (&croped, lineas);

		// Valida lineas obtenidas por hough según ancho de los sectores blancos detectados.
		// SDL: Filtro memoria, descarta si no son líneas estables.

		Validar_Lineas (lineas,lineas);

		// Imprime las lineas detectadas en el Frame analizado

		PrintLines (croped, lineas[0],255,0,0);
		PrintLines (croped, lineas[1],0,255,0);
		PrintLines (croped, lineas[2],0,0,255);

		 // Calculo de coordenadas en X - El vector que tiene los resultados de los calculos es abs.
		 // Para mostrar una salida, cada uno de estos resultados se imprime en pantalla.

		    for(i = 0 ; i < 3 ; i++){
		      if( lineas[i].rho && lineas[i].theta){
		    	  k++;
		      }
		    }

		    if((abs = (int*)malloc(sizeof(int)*k)) == NULL){
		    	perror("Error: No se puede reservar memoria\n");
		    }

		    CvPoint org;
		    CvScalar color = cvScalar(255,255,255);
		    k=0;
		    for (i=0;i<3;i++)
		    {
				if( lineas[i].rho && lineas[i].theta){
				abs[k] = Lineas_Xvalues (&lineas[i],150);
				sprintf(array, "ABS: %d",abs[k]);
				// Imprime en croped:
				org = cvPoint(10,k*20 + 20);
				cvPutText(croped, array, org, &font, color);
				k++;
				}
		    }
		    k=0;
		    free(abs);

		//---------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------
		    *draw = cvCreateImage(cvGetSize(croped), croped->depth , croped->nChannels);
		    cvCopy(croped, *draw, NULL);

		//cvShowImage( "video", draw );
	}
}

void
LineDetectEnd(void)
{
	  // Tidy up
	  cvDestroyWindow( "video" );
	  cvReleaseCapture( &capture );

	  // Libero memoria y retorno
	  cvReleaseImage(&croped);
}

/*
int main( int argc, char *argv[] ) {

  // Declaro variables
  int key =0;
  IplImage *croped = NULL;
  char *filename = NULL;
  Linea lineas[3];
  IplImage* frame = NULL;
  CvCapture* capture = NULL;
  int fps;

  // Check vargins
  if (argc < 3)
    help();

  else{
    if(strcmp(argv[1],"-v") == 0){

	filename = argv[2];
	capture = cvCaptureFromFile(filename);
	if ( !capture )
	{
	  fprintf (stderr,"No se pudo abrir el video\n" );
	  return 1;
	}
	frame = cvQueryFrame( capture );
	// Get the fps, needed to set the delay
	fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );
      }

      else if (strcmp(argv[1],"-c") == 0){

      //capture = cvCaptureFromCAM( CV_CAP_ANY );
      capture = cvCaptureFromCAM( 0 );
      fps = atoi(argv[2]);
      if ( !capture ) {
	fprintf( stderr, "ERROR al abrir camara\n" );
	exit(1);
      }

    }else{
      printf("\n Parametros invalidos\n");
      help();
      return 1;
    }
  }


  // Create a window to display the video

  cvNamedWindow("video", CV_WINDOW_AUTOSIZE);

  while( key != 'x' )
  {
    // get the image frame
    frame = cvQueryFrame( capture );

    if( !frame ) break;


 //-------------------------------------------------------------------------------------------------
 // Las lineas y llamadas que están entre estas lineas corresponden al soft de detección de lineas
 //-------------------------------------------------------------------------------------------------

//Recorte del Frame para obtener la ROI

    croped = CropImage (frame,50,350,350,150);

//Obtiene lineas utilizando transformada de Hough

    ImagesAndLines (&croped, lineas);

// Valida lineas obtenidas por hough según ancho de los sectores blancos detectados.
    // SDL: Filtro memoria, descarta si no son líneas estables.

    Validar_Lineas (lineas,lineas);

// Imprime las lineas detectadas en el Frame analizado

    PrintLines (croped, lineas[0],255,0,0);
    PrintLines (croped, lineas[1],0,255,0);
    PrintLines (croped, lineas[2],0,0,255);

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

    cvShowImage( "video", croped );

    // exit if user presses 'x'
    //key = cvWaitKey( 10000 / fps );
    key = cvWaitKey( 10 );
  }

  // Tidy up
  cvDestroyWindow( "video" );
  cvReleaseCapture( &capture );

  // Libero memoria y retorno
  cvReleaseImage(&croped);
  return 0;
}
*/


/**
 * @brief	Prinf Help. Imprime el help del algoritmo.
 * @arg		No recibe argumentos
 * @returns 	No retorna datos.
 * @author 	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	31/08/2014
 **/

void help(void){

  printf("\n-------------------------------------------------\n");
  printf("Por favor ingrese los parametros segun lo indicado:\n\n");
  printf("./exe videoInput [fps] [videoFile]\n\n");
  printf("videoInput:  -c = Camara ; -v = Video File\n\n");
  printf("[videoFile] = Si se elige la opcion video, en la\n\
linea deejecucion se debe indicar el PATH al archivo de\n\
video a analizar.\n\n");
  printf("[fps] = Si se elige la opcion de video por camara, se\n\
debe indicar la cantidad de fps de la misma.\n");
  printf("---------------------------------------------------\n\n");
  return;
}



