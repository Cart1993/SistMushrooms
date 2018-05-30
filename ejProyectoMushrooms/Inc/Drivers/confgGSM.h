
/**
* @file confgGSM.h
* @version 1.0
* @date 27/11/2017
* @author Christian Rangel
* @brief Configuracion de la red GSM para el modulo SIM800l
*
*/

#ifndef _confgGSM_H_
#define _confgGSM_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion para los estados de la red GSM
*
*/

typedef enum
{
	format,
	character,
	erase,
} eStateGSM;

/**
*	@brief Variable entera de 8 bits que almacena los reintentos de conexion por parte del modulo
*
*/

int8_t retryTimes;

/**
*	@brief Variable caso del tipo eStateGSM, la cual inicia el flujo del programa por format
*
*/

eStateGSM caso = format;

/**
*	@brief Array de enteros de 8 bits sin signo buffRx donde llega la data del modulo
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief initGSM Funcion de inicializacion del servicio GSM para el modulo SIM800L
*	@param void Vacio
*	@return int8_t Entero de 8 bits
*
*/

int8_t initGSM( void );


#endif /* _confgGSM_H_ */
