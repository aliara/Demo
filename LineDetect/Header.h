/**
 * \file	Funciones.h
 * \brief	Header de Funciones.c
 * \c		Compilar: make
 * \author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * \date 	20/07/2014
 **/


/**
 * \include	<stdio.h>
 * \include	<stdlib.h>
 * \include	<string.h>
 * \include	<errno.h>
 * \include 	<opencv/cv.h>
 * \include	<opencv/highgui.h>
 * \include 	<opencv/cxtypes.h>
 * \include 	<opencv/cxcore.h>
 * \include	<math.h>
 **/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include 	<opencv/cv.h>
#include	<opencv/highgui.h>
//#include 	<opencv/cxtypes.h>
//#include 	<opencv/cxcore.h>
#include	<math.h>


/**
 * \def		BGR		1		Nomenclatura OpenCV: 1 = tres colores (RGB)
 * \def		ANCHO_MAX	31		Máximo ancho para una línea válida (Modificar si se cambia la posicion de la camara)
 * \def		ANCHO_MIN	3		Mínimo ancho para una línea válida (Modificar si se cambia la posicion de la camara)
 * \def		CANT_FILAS	4		Numero de filas a analizar	(Se pueden agregar mas, en caso de agregar, hay que modificar el codigo)
 * \def		PI		3.14159		Constante Pi
 * \def		MAX_NUM_LINES	15		Maximo numero de lineas para el cuál pido memoria
 * \def		SE_DISCK	20		Parámetro de prefiltrado
 * \def		SE_DISCK_2	3		Parámetro de prefiltrado
 * \def		ANCHOR_X	1		Parámetro de prefiltrado
 * \def		ANCHOR_Y	1		Parámetro de prefiltrado
 * \def		KERNEL		3		Parámetro de prefiltrado
 * \def		XORDER		1		Parámetro de prefiltrado
 * \def		YORDER		0		Parámetro de prefiltrado
 * \def		rho_res 	1		Resolución de rho para la transformada de hough
 * \def		theta_res	CV_PI/60	Resolución de theta para la transformada de hough
 * \def		threshold	30		Valor minimo que debe alcanzar la votación dentro de la transformada de Hough
 * \def		min_length	0		Longitud minima del segmento a detectar
 * \def		max_gap		0		Salto maximo entre segmentos perteneciente a la misma recta
 * \def 	COND_CENTRO	(theta < 0+0.3 || theta >3.14-0.3)		Condición que debe cumplir para conciderarla como linea del centro
 * \def		COND_IZQ	(theta  > 0+0.3 && theta<1.3)			Condición que debe cumplir para conciderarla como linea izquierda
 * \def 	COND_DER	(theta < 3.14-0.3 && theta>2)			Condición que debe cumplir para conciderarla como linea derecha
 **/

#define		BGR		1
#define		ANCHO_MAX	8
#define		ANCHO_MIN	3
#define		CANT_FILAS	4
#define		PI		3.14159
#define		MAX_NUM_LINES	15
#define		SE_DISCK	20
#define		SE_DISCK_2	3
#define		ANCHOR_X	1
#define		ANCHOR_Y	1
#define		KERNEL		3
#define		XORDER		1
#define		YORDER		0

#define 	COND_CENTRO	(theta < 0+0.3 || theta >3.14-0.3)
#define		COND_IZQ	(theta  > 0+0.3 && theta<1.3)
#define 	COND_DER	(theta < 3.14-0.3 && theta>2)

//#define CROP_X 50
#define CROP_X 70
#define CROP_Y 350
//#define CROP_WIDTH 350
#define CROP_WIDTH 500
#define CROP_HEIGHT 150

#define		rho_res 	1
#define		theta_res	CV_PI/60
#define		threshold	30
#ifdef PROBABILISTICO
  #define		min_length	50
  #define		max_gap		20
#else
  #define		min_length	0
  #define		max_gap		0
#endif


/**
 * \struct	Punto
 * \brief	Punto de coordentadas X e Y
 * \details	Estructura que sirve para representar un pixel en una imágen
 * \date 	20/07/2014
 **/
typedef struct punto {

  int x;
  int y;

} Punto;


/**
 * \struct	Linea
 * \brief	Estructura que contiene todos los datos de una linea
 * \details	Para representar una línea se precisa de dos puntos o de una posicion y un ángulo
 * \var
 * 		punto1: Punto inicial de la recto
 * 		punto2: Punto final de la recto
 * 		theta: Angulo (en grados) de la recta
 * 		rho: Posicion de la recta
 * \date 	20/07/2014
 **/
typedef struct Linea {

  Punto punto1;
  Punto punto2;
  float rho;
  float theta;

} Linea;

void		ImagesAndLines		(IplImage**, Linea*);
void		Filtro_Lineas		(IplImage*, Linea*, Linea*);
void		AnalisisFilas		(IplImage*, int, int, int, int, int**, int**, int**, int**, int*, int*, int*, int*);
void		Analisis1Linea		(CvMat*, int**, int*);
int		Corroborar_Linea	(Linea*, int*, int, int);
int		Lineas_Xvalues		(Linea*, int);
IplImage*	CropImage 		(IplImage* image, int x1, int y1, int ancho, int alto);
void 		PrintLines 		(IplImage * img, Linea, int R , int G , int B);
void 		Validar_Lineas		(Linea* Line, Linea* Line_ret);
void		help			(void);
