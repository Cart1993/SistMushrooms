
/**
* @file humComp.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Soil Humidity
*	@brief Library to control the soil humidity
*	@description This library is to control the values of soil humidity
*
*/

#ifndef _humComp_H_
#define _humComp_H_

#include<stdint.h>
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



void controlHumComp( eFase x );

/**
*	@brief Variable externa de 16 bits sin signo hum1, hum2 (almacenan el valor de la humedad del suelo)
*
*/

extern uint16_t hum1, hum2;

/**
*	@brief Variable externa de 8 bits sin signo levelTank y cntHumComp para ordenar las prioridades de control
*
*/

extern uint8_t levelTank, cntHumComp, sensorOptic;

/**
*	@brief Function controlHumComp para el control de la humedad del suelo segun el periodo de cultivo
*	@param x Variable tipo eFase que indica en que periodo de cultivo se encuentra el sistema
  *	@return void
*
*/

void controlHumComp( eFase x );

#endif /* _humComp_H_ */
