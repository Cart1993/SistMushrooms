
/**
* @file humTemp.h
* @version 1.0
* @date 26/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Humidity Temperature Ambiental
*	@brief Library to control the ambiental humidity and temperature
*	@description This library is to control the values of ambiental humidity and the ambiental temperature
*
*/

#ifndef _humTemp_H_
#define _humTemp_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "STM_MY_LCD16X2.h"
#include "DHT11.h"

/**
*	@brief Estructura de enumeracion eCase la cual indica que estado de operacion resulto de la medicion del DHT11
*
*/

typedef enum
{
	readSuccesfully = 0,
	notReady,
	checksumError,
	retryLater,
}eCase;

/**
*	@brief Variable externa de 8 bits sin signo humidity1 (almacena la parte entera de la medicion de la humedad relativa del ambiente)
*	@brief Variable externa de 8 bits sin signo humidity12 (almacena la parte decimal de la medicion de la humedad relativa del ambiente)
*	@brief Variable externa de 8 bits sin signo temperature1 (almacena la parte entera de la medicion de la temperatura del ambiente)
*	@brief Variable externa de 8 bits sin signo temperature2 (almacena la parte decimal de la medicion de la humedad relativa del ambiente)
*
*/

extern uint8_t humidity1, humidity2, temperature1, temperature2;

/**
*	@brief Function disp_enviar para el envio de mensajes a traves del display LCD 16x2
*	@param text Mensaje a desplegar a traves del display LCD
*	@param linea Línea en la cual se va a desplegar el mensaje en el display
*	@return void
*
*/

extern void disp_enviar( char* text, int linea );

/**
*	@brief Function getTempHum Toma el valor de humedad y temperatura para que lo despliegues en la LCD
*	@param x Indica de cual sensor obtener las mediciones
*	@return void
*
*/

void getTempHum( uint8_t x );


#endif /* _humTemp_H_ */
