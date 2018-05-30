
/**
* @file fase.h
* @version 1.0
* @date 2/9/2017
* @author Christian Rangel
* @brief Fases de cultivacion del champiñon
*
*/

#ifndef _fase_H_
#define _fase_H_

#include <stdint.h>

/**
*	@brief Estructura de enumeracion para las fases de cultivacion en la que se encuentran los champiñones
*
*/

typedef enum
{
  error = -1,
  faseGerminacion = 0,
  fasePreFructificacion,
  faseIndFructificacion,
  fasePlFructificacion,
  faseResFructificacion,
  faseMax,
}eFase;

/**
*	@brief Array externo entero de 8 bits en el cual se encuentra la fecha almacenada de la siembra.
*
*/

extern uint8_t Date[4];

/**
*	@brief fase Funcion que controla las fases de cultivacion del sistema
*	@param date Periodo de cultivacion
*	@return eFase El periodo de cultivacion
*
*/

eFase fase( eFase date);

#endif /* _fase_H_ */
