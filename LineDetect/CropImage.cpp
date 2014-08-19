/**
 * @brief	Función que recorta la imagen y se queda solo con la parte útil
 * @arg		Se pasa como argumento la imagen y los límites de interés
 * @param image Frame a recortar.
 * @param 	x Coordenada X Inicio de corte.
 * @param 	y Coordenada Y Inicio de corte.
 * @param width Ancho del corte.
 * @param height Alto del corte.
 * @return	La función devuelve la imagen recortada
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	24/07/2014
 **/

#include "Header.h"

IplImage*  CropImage (IplImage* image,int x,int y,int width, int height)
{
    IplImage *Cropped;
    CvRect region;

    region.x=x;
    region.y=y;
    region.width=width;
    region.height=height;

    cvSetImageROI((IplImage*)image, region);

    Cropped = cvCreateImage(cvGetSize(image), image->depth , image->nChannels);
    cvCopy(image, Cropped,NULL);

    cvResetImageROI((IplImage*)image);

    return Cropped;

}




