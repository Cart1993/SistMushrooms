
/**
* @file tempMCU.h
* @version 1.0
* @date 29/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Temperature of the MCU
*	@brief Library to control the temperature of the microcontroller
*	@description This library is to control the values of the temperature of the soil at the microcontroller
*
*/

#ifndef _tempMCU_H_
#define _tempMCU_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "STM_MY_LCD16X2.h"

/**
*	@brief Constante tam que define la cantidad de canales que se estan ejecutando en el ADC del MCU
*
*/

#define tam 6

/**
*	@brief Variable externa de 8 bits sin signo llamada alertaTempMcuHigh que activa la alarma por temperatura excesiva del MCU
*
*/

extern uint8_t alertaTempMcuHigh, process;

/**
*	@brief Array externo tipo char llamada alertaTempMCU en donde esta el mensaje de alerta a enviar a traves del SIM800L
*
*/

extern char alertaTempMCU[];

/**
*	@brief Array externo de 8 bits sin signo llamado buffer en donde se almacenan los valores del ADC
*
*/

extern uint8_t buffer[ tam ];

/**
*	@brief Variable externa de 16 bits llamada tempSensorMCU que almacena el valor de la temperatura del MCU
*
*/

extern int16_t tempSensorMCU;

/**
*	@brief Variables externas tipo flotante V25, Avg_Slope que convierten el valor obtenido a niveles de grados celsius
*
*/

extern float V25, Avg_Slope;

/**
*	@brief Function extern dips_enviar Envia mensajes a traves del display LCD de 16x2
*	@param text Variable tipo char que contiene el mensaje a desplegar en la display
*	@param linea Variable tipo entera que indica en que linea del display se mostrara la informacion
*	@return void
*
*/

extern void disp_enviar( char* text, int linea );

/**
*	@brief Function tempSensor mide y reporta alarmas con respecto a la temperatura de la MCU
*	@param void
*	@return void
*
*/

void tempSensor( void );

#endif /* _tempMCU_H_ */
