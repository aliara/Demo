
/**
 * @brief	Devuelve para una sola fila en qué posicion en X puede haber lineas de un espezor definido
 * @arg		Se pasa como argumento la fila (F) a analizar
 * @param Filas Fila a analizar
 * @param Maximos Vector donde se guardan los maximos detectados.
 * @param SizeM Cantidad de Maximos detectados. Variable modificada por referencia.
 * @return	Devuelve el vector de máximos (Max) y la cantidad de ellos (SizeM)
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	20/07/2014
 **/

#include "Header.h"

// Analiza sobre 4 filas, en la imagen bw obtenida antes de aplicar el canny.
// Convoluciona con [1 1 1 1 1 1] cada fila para obtener posible líneas.
void Analisis1Linea(CvMat *Filas, int **Maximos, int *SizeM) {

  // Declaración de variables
  CvMat *Fila_Conv = NULL;
  int *Maximos_aux = NULL;
  int *Maximos_aux2 = NULL;
  int Delta = ANCHO_MAX/2;
  int Valor = 0;
  int Valor_Max = ANCHO_MAX * 15 * 2;
  int centro = (ANCHO_MAX -1)/2;
  int i, j, x;
  unsigned width = Filas->cols;

  // Pedido de memoria
  Fila_Conv = (CvMat*)malloc (sizeof(CvMat));

  if ( Fila_Conv == NULL  ) {
    fprintf(stderr, "\nError de malloc\nCódigo de error: %s\n", strerror(errno));
    exit (1);
  }

  Fila_Conv->data.i = (int*)malloc (sizeof(int)*width);
  Maximos_aux = (int*)malloc (sizeof(int)*Filas->cols);
  Maximos_aux2 = (int*)malloc (sizeof(int)*Filas->cols);

  if ( Fila_Conv->data.i == NULL  || Maximos_aux == NULL || Maximos_aux2 == NULL) {
    fprintf(stderr, "\nError de malloc\nCódigo de error: %s\n", strerror(errno));
    exit (1);
  }

  // Convoluciono con ones(1,ANCHO_MAX)
  for (x = 0; x < width; x++) {
    Fila_Conv->data.i[x] = 0;
    for (i = -centro; i < (centro+1); i++) {

      if ((x+i) < 0 || (x+i) >= width)
	Fila_Conv->data.i[x] += 0;
      else
	Fila_Conv->data.i[x] += Filas->data.ptr[x+i];

    }
    Fila_Conv->data.i[x] /= 255;
  }

  // Encuentro posibles máximos
  i = 0;
  for (x = 0; x < width; x++) {
    if (ANCHO_MIN < Fila_Conv->data.i[x] && Fila_Conv->data.i[x] < (ANCHO_MAX+1)) {
      Maximos_aux[i] = x;
      i++;
    }
  }
  *SizeM = i;

  // Reduzco la cantidad de máximos
  if (*SizeM != 0) {
    i=0;
    x=0;
    while ((i != *SizeM)) {

      j=0;
      while ( (j < *SizeM-i) && (Maximos_aux[i] == Maximos_aux[i+j]-j) ) 	j++;

      Maximos_aux2[x] = Maximos_aux[(int)(i + j/2)];
      i += j;
      x++;
    }
    *SizeM = x;
  }

  *Maximos = (int*)malloc (sizeof(int)*(*SizeM));

  // Analizo que no sean falsos máximos
  j=0;
  if (*SizeM != 0) {
    for (i=0; i < *SizeM; i++) {

      if ( (Maximos_aux2[i] > Delta) && ((Maximos_aux2[i] + Delta) < Filas->cols) ) {

	Valor = 0;
	for (x = (Maximos_aux2[i]-Delta); x <= (Maximos_aux2[i]+Delta); x++)
	  Valor += Fila_Conv->data.i[x];

	if ( Valor < Valor_Max ) {
	  (*Maximos)[j] = Maximos_aux2[i];
	  j++;
	}
      }
    }
    *SizeM = j;
  }

  *Maximos = (int*)realloc(*Maximos,sizeof(int)*(*SizeM));

  // Devuelvo la memoria y me voy
  free(Maximos_aux);
  free(Maximos_aux2);
  free(Fila_Conv->data.i);
  free(Fila_Conv);
  return;
}






