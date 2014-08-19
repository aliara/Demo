


#include "Header.h"

#define 	VAL_MIN_FRAMES		4
#define		VAL_NO_LINE		6

#define		DELTA_RHO		10
#define		DELTA_THETA		0.15

void LimpiarLinea ( Linea * line)
{
  line->punto1.x=0;
  line->punto1.y=0;
  line->punto2.x=0;
  line->punto2.y=0;
  line->rho=0;
  line->theta=0;
}

int  CompareLines (Linea *Line1,Linea * Line2 )
{
  float delta_rho = Line1->rho - Line2->rho;
  float delta_theta = Line1->theta - Line2->theta;

  if ( delta_rho < DELTA_RHO && delta_rho > -DELTA_RHO)
  {
    if ( delta_theta < DELTA_THETA && delta_theta > -DELTA_THETA)
    {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief	Valida las lineas
 * @param line	Vector con lineas a validar
 * @param line_ret Vector con lineas validadas. Resultado de la función
 * @return	No retorna nada.
 * @author	Santiago F. Maudet - Andrés Demski - Nicolás Linale
 * @date 	26/07/2014
 **/

void Validar_Lineas (Linea * line , Linea * line_ret)
{

  static Linea  Lineas_Estables[3] ={ {{0,0},{0,0},0,0},{{0,0},{0,0},0,0},{{0,0},{0,0},0,0} };
  static Linea  Lineas_Inestables[3] = { {{0,0},{0,0},0,0},{{0,0},{0,0},0,0},{{0,0},{0,0},0,0} };
  static  int   Cnt_inestables[3] = { 0,0,0 };
  static  int   Cnt_NoLine[3] = { 0,0,0 };

  int i=0;

  for (i=0;i<3;i++)
  {
      if (line[i].rho) //Si se detecto linea
      {
	Cnt_NoLine[i]=0;
	if ( CompareLines (&line[i],&Lineas_Estables[i]))
	{
	  Lineas_Estables[i]=line[i];
	}

	else
	{
	  if ( CompareLines(&line[i],&Lineas_Inestables[i]) )
	  {
	    Lineas_Inestables[i] = line[i];
	    Cnt_inestables[i] += 1;
	    if ( Cnt_inestables[i] >= VAL_MIN_FRAMES)
	    {
	      Lineas_Estables[i]= line[i];
	      Cnt_NoLine[i]=0;
	    }
	  }

	  else
	  {
	    Lineas_Inestables[i] = line[i];
	    Cnt_inestables[i]= 0;
	  }

	}
      }
      else  //No se detecto linea
      {
	if (Cnt_NoLine[i] >= VAL_NO_LINE )
	{
	  LimpiarLinea (&Lineas_Estables[i]);
	}
	else Cnt_NoLine[i] += 1;
      }
  }


  if ( CompareLines (&Lineas_Estables[1],&Lineas_Estables[0]) )
  {
    LimpiarLinea (&Lineas_Estables[0]);
  }

  if ( CompareLines (&Lineas_Estables[1],&Lineas_Estables[2]) )
  {
    LimpiarLinea (&Lineas_Estables[2]);
  }

  for (i=0;i<3;i++)
  {
    line_ret[i] = Lineas_Estables[i];
  }

  return;

}

