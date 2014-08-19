/**
 * @brief	Verifica si las lineas de Hough coinciden con los máximos
 * @arg		Se pasa como argumento las líneas, los máximos, la cantidad de maximos y la Fila.
 * @param linea Vector de lineas a analizar
 * @param Maximos Maximos de transformada de Hough
 * @param Fila  Fila a analizar
 * @param SizeM Cantidad de Maximos.
 * @return	La función devuelve un 1 en caso de encontrar coincidencia y un 0 en caso contrario.
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	21/07/2014
 **/
#include "Header.h"

int Corroborar_Linea(Linea *linea, int *Maximos, int Fila, int SizeM) {

  // Declaro variables
  int X_Value;
  int i;

  // Obtengo los valores en X
  X_Value = Lineas_Xvalues(linea, Fila);


  // Corroboro la validez de la linea
  if ( SizeM > 0 && SizeM < 4) {
    for (i = 0; i < SizeM; i++) {
      if ( (X_Value - Maximos[i]) < 10 || (Maximos[i] - X_Value) < 10 )	return 1;
    }
  }

  // Si ninguna linea es válida, devuelvo 0
  return 0;
}



