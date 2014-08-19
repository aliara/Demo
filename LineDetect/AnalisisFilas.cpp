/**
 * @brief	Devuelve para 4 filas distintas en qué posición en X puede haber lineas de un espezor definido
 * @arg		Se pasa como argumento el Frame (black & white) y las filas a analizar (F)
 * @return	La función devuelve los máximos de cada fila (Max) y su correspondiente número de elementos (SizeM)
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	20/07/2014
 **/

#include "Header.h"

void AnalisisFilas(IplImage *Croped_Frame, int F1, int F2, int F3, int F4, int **Max1, int **Max2, int **Max3, int **Max4, int *SizeM1, int *SizeM2, int *SizeM3, int *SizeM4) {

  // Declaración de variables
  CvMat *Filas = NULL;

  // Pedido de memoria
  Filas = (CvMat*)malloc(sizeof(CvMat)*CANT_FILAS);

  if ( Filas == NULL  ) {
    fprintf(stderr, "\nError de malloc\nCódigo de error: %s\n", strerror(errno));
    exit (1);
  }

  // Extraigo las filas que me interesan
  cvGetRow(Croped_Frame, &(Filas[0]), F1);
  cvGetRow(Croped_Frame, &(Filas[1]), F2);
  cvGetRow(Croped_Frame, &(Filas[2]), F3);
  cvGetRow(Croped_Frame, &(Filas[3]), F4);

  // Analizo fila por fila
  Analisis1Linea(&(Filas[0]), Max1, SizeM1);
  Analisis1Linea(&(Filas[1]), Max2, SizeM2);
  Analisis1Linea(&(Filas[2]), Max3, SizeM3);
  Analisis1Linea(&(Filas[3]), Max4, SizeM4);


  // Devuelvo la memoria y me voy

  free(Filas);
  return;
}






