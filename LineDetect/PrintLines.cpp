/**
 * @brief	La función muestra las líneas detectadas sobre la imagen - RGB son los parametros que dan color a la linea.
 * @param img	 Frame a imprimir
 * @param lineas Vector con lineas detectadas
 * @param R 	Color Rojo
 * @param G	Color Verde
 * @param B	Color Azul
 * @return	No retorna nada
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	24/07/2014
 **/

#include "Header.h"

void PrintLines (IplImage * img, Linea lineas, int R , int G , int B)
{
    CvPoint pt1, pt2;
    pt1.x = lineas.punto1.x;
    pt1.y = lineas.punto1.y;
    pt2.x = lineas.punto2.x;
    pt2.y = lineas.punto2.y;
    cvLine(img, pt1, pt2, CV_RGB(R,G,B), 3, 8, 0);

    return;
}


