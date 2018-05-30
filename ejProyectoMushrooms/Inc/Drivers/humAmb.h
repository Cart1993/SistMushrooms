
/**
* @file humAmb.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Humidity Ambiental
*	@brief Library to control the ambiental humidity
*	@description This library is to control the values of ambiental humidity
*
*/

#ifndef _humAmb_H_
#define _humAmb_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fase.h"
#include "AT.h"
#include "tempAmb.h"

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

extern uint8_t countHum;

/**
*	@brief Variable externa de 8 bits sin signo humidity1 (almacena el valor de la humedad) y cntHum (para la prioridad del control).
*
*/

extern uint8_t humidity1, cntHum, alertaHumLow;

/**
*	@brief Function controlHumAmb para el control de la humedad ambiental segun el periodo de cultivo
*	@param x Variable tipo eFase que indica en que periodo de cultivo se encuentra el sistema
*	@return void
*
*/

void controlHumAmb( eFase x );

#endif /* _humAmb_H_ */
