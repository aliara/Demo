
/**
 * @brief	Función que valida las lineas arrojadas por Hough
 * @arg		Se pasa como argumento un puntero al Frame, las lineas a validar y la cantidad total de lineas
 * @param image  Frame a analizar.
 * @param lineas Lineas a validar.
 * @param lineas_ret Lineas validadas.
 * @return	Devuelve por referencia las lineas validadas y la cantidad de lineas validadas (ambos datos pasados por argumento)
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	20/07/2014
 **/

#include "Header.h"

void Filtro_Lineas (IplImage *image, Linea *lineas, Linea *lineas_ret) {

  // Declaración de variables
  int F1, F2, F3, F4;
  int *Max1 = NULL, *Max2 = NULL, *Max3 = NULL, *Max4 = NULL;
  int SizeM1, SizeM2, SizeM3, SizeM4;
  int Linea_Ok;
  int i;

  // Defino la ubicación de cada Fila
  F1 = image->height * 3/8;
  F2 = image->height * 4/8;
  F3 = image->height * 5/8;
  F4 = image->height * 6/8;

  // SDL: Analizo las filas convolucionando.
  AnalisisFilas(image, F1, F2, F3, F4, &Max1, &Max2, &Max3, &Max4, &SizeM1, &SizeM2, &SizeM3, &SizeM4);

  // Valido las lineas
  // SDL: Corrobora que los máximos se correspondan con las 3 líneas encontradas.
  for (i=0; i < 3; i++) {
    Linea_Ok = 0;

    if (Corroborar_Linea( &(lineas[i]), Max1, F1, SizeM1 ))
      Linea_Ok++;
    if (Corroborar_Linea( &(lineas[i]), Max2, F2, SizeM2 ))
      Linea_Ok++;
    if (Corroborar_Linea( &(lineas[i]), Max3, F3, SizeM3 ))
      Linea_Ok++;
    if (Corroborar_Linea( &(lineas[i]), Max4, F4, SizeM4 ))
      Linea_Ok++;

    if (Linea_Ok > 1) {
      lineas_ret[i].punto1 = lineas[i].punto1;
      lineas_ret[i].punto2 = lineas[i].punto2;
      lineas_ret[i].theta = lineas[i].theta;
      lineas_ret[i].rho = lineas[i].rho;
    }
    else
    {
      lineas_ret[i].punto1.x = 0;
      lineas_ret[i].punto1.y = 0;
      lineas_ret[i].punto2.x = 0;
      lineas_ret[i].punto2.y = 0;
      lineas_ret[i].theta = 0;
      lineas_ret[i].rho = 0;
    }
  }


  // Devuelvo la memoria y me voy
  free(Max1);
  free(Max2);
  free(Max3);
  free(Max4);
  return;
}


