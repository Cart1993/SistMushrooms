
/**
* @file funcionInicializacion.h
* @version 1.0
* @date 6/9/2017
* @author Christian Rangel
* @brief Inicializacion y configuracion del modulo SIM800L
*
*/

#ifndef _funcionInicializacion_H_
#define _funcionInicializacion_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion para las fases las etapas de inicializacion del modulo
*
*/

typedef enum
{
	EchoMode,
	connection,
	BaudRate,
	TempSim,
	functionality,
	registration,
	operator,
} eState;

/**
*	@brief Variable entera de 8 bits llamada intento, que lleva la cuenta de los intentos de la inicializacion
*
*/

int8_t intento;

/**
*	@brief Array externo de 8 bits sin signo buffRx que almacena todo lo que responde el modulo al MCU
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief Variable llamada ctrl del tipo eState que inicia el flujo de la inicializacion en EchoMode
*
*/

eState ctrl = EchoMode;

/**
*	@brief Function Inicializacion
*	@brief Inicializa el funcionamiento del modulo SIM800L
*	@param void Vacio
*	@return int8_t Entero de 8 bits (0---> Good Initialization, 1---> Bad Initialization)
*
*/

int8_t Inicializacion( void );

#endif /* _funcionInicializacion_H_ */
