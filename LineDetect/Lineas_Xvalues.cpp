/*
 /**
 * @brief	Función que obtiene el valor en el eje X de las lineas para cada Fila
 * @arg		Se pasa como argumento una linea y una Fila.
 * @param linea Vector de lineas
 * @param Y  	Fila a analizar.
 * @return	La función devuelve la coordenada en el eje X de la linea detectada
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	21/07/2014
 **/

#include "Header.h"

int Lineas_Xvalues(Linea *linea, int Y) {
  double a = cos (linea->theta);
  double b = sin (linea->theta);
  return (linea->rho -(Y* b))/a;
}


// int Lineas_Xvalues(Linea *linea, int Y) {
//
//   // Declaro variables
//   unsigned Xo = 0, Yo = 0;
//   float m;
//   int theta;
//   int X;
//
//   // Busco las coordenadas en el eje X
//     Xo = linea->punto1.x;
//     Yo = linea->punto1.y;
//     theta = linea->theta;
//     m = - cos(theta * PI / 180) / sin(theta * PI / 180);
//     X = ((Y - Yo)/m) + Xo;
//
//   return X;
// }
