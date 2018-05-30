
/**
* @file ctrlFase.h
* @version 1.0
* @date 2/9/2017
* @author Christian Rangel
* @brief Control de todas las fases de cultivacion del champiñon
*
*/

#ifndef _ctrlFase_H_
#define _ctrlFase_H_

#include<stdint.h>
#include "fase.h"

/**
*	@brief Variable externa entera sin signo de 8 bits llamada setDate para fijar la fecha inicial y a partir de alli realizar el control
*
*/

extern uint8_t setDate;

/**
*	@brief Variable externa entera sin signo de 8 bits llamada setMonth para fijar el mes inicial y a partir de alli realizar el control
*
*/

extern uint8_t setMonth;

/**
*	@brief cultivation Funcion que controla los periodos de cultivacion del sistema
*	@param date La fecha en la que se encuentra el sistea. La variable es de tipo eFase
*	@return eFase
*
*/

eFase cultivation( eFase date );

#endif /* _ctrlFase_H_ */
