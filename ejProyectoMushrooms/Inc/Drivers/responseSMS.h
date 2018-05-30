
/**
* @file responseSMS.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library response the messages
*	@brief Library for response the messages through SIM800L
*	@description This library is to apply the response SMS text on SIM800L
*
*/

#ifndef _responseSMS_H_
#define _responseSMS_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion eMsj la cual indica en que estado se encuentra, en envio de mensaje o recepcion de mensajes 
*
*/

typedef enum
{
	fail,
	smsRX,
	smsTX,
}eMsj;

/**
*	@brief Variables externas enteras de 8 bits sin signo que indican las alertas que se activan para atenderlas
*
*/

extern uint8_t alertaHumLow, alertaTempLow, alertaTempHigh, alertaTempMcuHigh, countTemp, countHum;

/**
*	@brief Array externo de 8 bits sin signo buffRx donde se almacena la data enviada y recibida al MCU
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief Array externo tipo char donde se almacena el numero telefonico de donde se recibio el msj
*
*/

extern char numeroTelefonico[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la temperatura ambiental a enviar a traves de SMS
*
*/

extern char temperaturaAmbiental[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la humedad ambiental a enviar a traves de SMS
*
*/

extern char humedadAmbiental[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la temperatura del sustrato a enviar a traves de SMS
*
*/

extern char temperaturaSustrato[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la humedad del sustrato a enviar a traves de SMS
*
*/

extern char humedadSustrato[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la iluminacion a enviar a traves de SMS
*
*/

extern char iluminacion[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de los niveles del tanque a enviar a traves de SMS
*
*/

extern char nivelTanque[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la temperatura del modulo SIM800L a enviar a traves de SMS
*
*/

extern char temperaturaSim800[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la temperatura del MCU a enviar a traves de SMS
*
*/

extern char temperaturaMCU[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de todas las variables implicadas en el sistema a enviar a traves de SMS
*
*/

extern char msjAllVariables[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de error de codigo a enviar a traves de SMS
*
*/

extern char errorMsj[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de error de codigo a enviar a traves de SMS
*
*/

extern char alertaTempAlta[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la alerta de temperatura ambiental alta a enviar a traves de SMS
*
*/

extern char alertaTempBaja[];

/**
*	@brief Array externo tipo char donde se almacena el despliegue de la informacion de la alerta de humedad ambiental baja a enviar a traves de SMS
*
*/

extern char alertaHumBaja[];

/**
*	@brief Array externo tipo char donde se almacena el numero telefonico al cual llegaran las alarmas emitidas por el sistema
*
*/

extern char numeroTelefonicoAlertas[];

/**
*	@brief Array externo tipo char donde se almacena el mensaje de alerta para temperatura excesiva de la MCU
*
*/

extern char alertaTempMCU[];


/**
*	@brief Function Send envia el mensaje correspondiente al numero que solicito la informacion
*	@param _number Número telefonico en donde se enviara la data
*	@param _message Mensaje a enviar al dispositivo
*	@return void
*
*/

extern void Send( char *_number, char *_message );

/**
*	@brief Function SMSquery consulta de mensaje, si es para enviar o para recibir mensajes
*	@param receiveMsj Variable que indica si se necesita leer o enviar mensaje
*	@return void
*
*/

void SMSquery( uint8_t receiveMsj );

#endif /* _responseSMS_H_ */
