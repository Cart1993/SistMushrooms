
/**
* @file light.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library Level of the illumination
*	@brief Library to control the illumination levels
*	@description This library is to control the values of level of the illumination
*
*/

#ifndef _light_H_
#define _light_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
*	@brief Estructura de enumeracion eLight la cual indica en que estado se encuentra la iluminacion del entorno del cultivo
*
*/

typedef enum{
  LightError = -1,
  LightLow = 0,
  LightMedium,
  LightHigh,
}eLight;

/**
*	@brief Function getLight verifica y devuelve el estado actual de la iluminacion del entorno del cultivo
*	@param void 
*	@return eLight Estado actual de iluminacion del sistema
*
*/

eLight getLight( void );

#endif /* _light_H_ */
