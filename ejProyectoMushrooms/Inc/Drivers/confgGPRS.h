
/**
* @file confgGPRS.h
* @version 1.0
* @date 27/11/2017
* @author Christian Rangel
* @brief Configuracion de la red GPRS para el modulo SIM800l
*
*/


#ifndef _confgGPRS_H_
#define _confgGPRS_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion para los estados de la red GPRS
*
*/

typedef enum
{
	SERVICE,
	REGISTRATION,
	SOCKET,
	WIRELESS,
	LOCALIP,
	BEARER,
} eStateGPRS;

/**
*	@brief Variable entera de 8 bits que almacena los reintentos de conexion por parte del modulo
*
*/

int8_t retry;

/**
*	@brief Variable GPRS del tipo eStateGPRS, la cual inicia el flujo del programa por SERVICE
*
*/

eStateGPRS GPRS = SERVICE;

/**
*	@brief Array de enteros de 8 bits sin signo buffRx donde llega la data del modulo
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief initGPRS Funcion de inicializacion del servicio GPRS para el modulo SIM800L
*	@param void Vacio
*	@return int8_t Entero de 8 bits
*
*/

int8_t initGPRS( void );


#endif /* _confgGPRS_H_ */
