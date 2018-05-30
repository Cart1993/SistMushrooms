
/**
* @file controlAmbiental.h
* @version 1.0
* @date 10/10/2017
* @author Christian Rangel
* @brief Control de todas las variables del sistema
*
*/

#ifndef _controlAmbiental_H_
#define _controlAmbiental_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fase.h"
#include "Light.h"
#include "humComp.h"
#include "tempComp.h"
#include "tempMCU.h"

/**
*	@brief Definicion de una constante "desocupado" igual a 0 (desocupado el sistema para controlar)
*
*/

#define desocupado 0

/**
*	@brief Definicion de una constante "ocupado" igual a 1 (ocupado el sistema para controlar)
*
*/

#define ocupado 1

/**
*	@brief Estructura de enumeracion para los estados del sistema
*
*/

#define timeout 2

typedef enum
{
	OFF,
	ON,
	RUN,
} eSystem;

/**
*	@brief Estructura de enumeracion para los estados de los niveles del tanque
*
*/

typedef enum
{
	VOID = 0,
	MEDIUM,
	FULL,
	FALSE,
}eLevel;

/**
*	@brief Variable externa de tipo eFase llamada "date"
*
*/

extern eFase date;

/**
*	@brief Variable externa llamada system del tipo eSystem
*
*/

extern eSystem system;

/**
*	@brief Funcion externa de tipo eLight llamada Get_Light
*	@param void Vacio
*	@return eLight Valor de tipo eLight (Estado de la iluminacion)
*
*/

extern eLight Get_Light( void );

/**
*	@brief Funcion externa de tipo void controlTempAmb la temperatura ambiental
*	@param eFase variable que indica el periodo en el que se encuentra el sistema
*	@return void
*
*/

extern uint8_t lightHigh, lightMedium;

extern void controlTempAmb( eFase x );

/**
*	@brief Variables externas llamadas sensorOptic, levelTank, cntLight. Todas del tipo entero de 8 bits sin signo.
*
*/

extern uint8_t sensorOptic, levelTank, g_countTank, cntLight, g_countLight;

/**
*	@brief Funcion externa de tipo void controlHumAmb que controla la humedad ambiental 
*	@param eFase variable que indica el periodo en el que se encuentra el sistema
*	@return void
*
*/

extern void controlHumAmb( eFase x );

/**
*	@brief controlAmbiental Funcion que controla las variables del sistema
*	@param void Vacio
*	@return void Vacio
*
*/

void controlAmbiental( void );

#endif /* _controlAmbiental_H_ */
