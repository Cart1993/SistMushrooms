
/**
* @file tempComp.h
* @version 1.0
* @date 20/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Soil Temperature
*	@brief Library to control the soil temperature
*	@description This library is to control the values of soil temperature
*
*/

#ifndef _tempComp_H_
#define _tempComp_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fase.h"

/**
*	@brief Constante desocupado 0, para poder realizar control.
*
*/

#define desocupado 0

/**
*	@brief Constante ocupado 1, para poder realizar control.
*
*/

#define ocupado 1

/**
*	@brief Variables externa entera de 16 bits sin signo tempComp1 y tempComp2 que almacenan el valor de la temperatura del sustrato del sistema
*
*/

extern uint16_t tempComp1, tempComp2;

/**
*	@brief Variables externa entera de 8 bits sin signo levelTank, cntAA y cntHumComp que llevan el orden de prioridad para la realizacion de control del cultivo
*
*/

extern uint8_t levelTank, cntAA, cntHumComp, sensorOptic;

/**
*	@brief Function extern ReportTemperature la cual mide y reporta el valor de la temperautra obtenido a traves del sensor DS18B20
*	@param void
*	@return uint16_t Variable de 16 bits sin signo (Si la medicion es correcta devuelve el valor medido de lo contrario devuelve 200)
*
*/

extern uint16_t ReportTemperature ( void );

/**
*	@brief Function extern OneWire_Init Inicializacion de un timer del MCU y del sistema para leer el sensor
*	@param void
*	@return void
*
*/

extern void OneWire_Init( void );

/**
*	@brief Function controlTempComp Controla y mide la temperatura del suelo del cultivo
*	@param x Variable que indica el estado del cultivo o el periodo
*	@return void
*
*/

void controlTempComp( eFase x );

#endif /* _tempComp_H_ */
