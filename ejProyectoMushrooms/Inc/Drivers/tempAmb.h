
/**
* @file tempAmb.h
* @version 1.0
* @date 21/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Humidity and Temperature Ambiental
*	@brief Library to control the ambiental humidity and temperature
*	@description This library is to control the values of ambiental humidity and ambiental temperature
*
*/

#ifndef _tempAmb_H_
#define _tempAmb_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fase.h"
#include "DHT11.h"
#include "AT.h"

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
*	@brief Constante timeout 20, para culminar los intentos de equilibrio tanto en la temperatura como en la humedad
*
*/

#define timeout 20

/**
*	@brief Variable externa de 8 bits sin signo que lleva el conteo de la tolerancia de una variable fuera de rango
*
*/

extern uint8_t countTemp;

/**
*	@brief Variable externa de 8 bits sin signo que activa la alerta por temperaturas y/o humedad elevadas y bajas
*
*/

extern uint8_t alertaHumLow, alertaTempLow, alertaTempHigh;

/**
*	@brief Variables externas de 8 bits sin signo
*	@brief humidity1 (almacena la parte entera de la medicion de humedad ambiental) y humidity2 (almacena la parte decimal de la humedad ambiental)
*	@brief temperature1 (almacena la parte entera de la medicion de la temperatura ambiental) y temperature2 (almacena la parte decimal de la temperatura ambiental)
*	@brief cntAA, cntLight y cntHum es para llevar el orden de prioridades de control del sistema
*
*/

extern uint8_t humidity1, humidity2, temperature1, temperature11, temperature2, cntAA, cntLight, cntHum, alertaTemp;

/**
*	@brief Function controlTempAmb para el control de la temperatura ambiental segun el periodo de cultivo
*	@param x Variable tipo eFase que indica en que periodo de cultivo se encuentra el sistema
*	@return void
*
*/

void controlTempAmb( eFase x );

#endif /* _tempAmb_H_ */
