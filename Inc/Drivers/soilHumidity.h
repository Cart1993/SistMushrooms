
/**
* @file soilHumidity.h
* @version 1.0
* @date 8/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Soil Humidity
*	@brief Library to control the soil humidity through hygrometer
*	@description This library is to control the values of the soil humidity
*
*/

#ifndef _soilHumidity_H_
#define _soilHumidity_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
*	@brief Constante tam que define la cantidad de canales del ADC que se estan usando
*
*/

#define tam 6

/**
*	@brief Estructura de enumeracion eHum la cual indica en que estado se encuentra el sistema con respecto a la humedad del suelo
*
*/

typedef enum{
  humError = -1,
  humLow = 0,
  humMedium,
  humMediumHigh,
  humHigh,
}eHum;

/**
*	@brief Variables enteras de 16 bits sin signo, hum1 y hum2 (almacenan los valores que miden los higrometros) y humTotal que es el valor promediado de la humedad
*
*/

extern uint16_t hum1, hum2, humTotal;

/**
*	@brief Array externo de 8 bits sin signo llamado buffer en donde se almacenan todos los valores que mide el ADC del MCU
*
*/

extern uint8_t buffer[ tam ];

/**
*	@brief Function getSoilHumidity toma los valores de la humedad del suelo a traves del ADC y retorna el estado del sistema
*	@param void 
*	@return eHum Estado del sistema con respecto a la humedad del suelo
*
*/

eHum getSoilHumidity( void );

#endif /* _soilHumidity_H_ */
