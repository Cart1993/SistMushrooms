
/**
* @file sendMessage.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library send the message
*	@brief Library for send the messages through SIM800L
*	@description This library is to apply the send SMS text on SIM800L
*
*/

#ifndef _sendMessage_H_
#define _sendMessage_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Array externo de 8 bits sin signo buffRx donde se almacena la data enviada y recibida al MCU
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief Function Send envia el mensaje correspondiente al numero que solicito la informacion
*	@param _number Número telefonico en donde se enviara la data
*	@param _message Mensaje a enviar al dispositivo
*	@return void
*
*/

void Send( char *_number, char *_message );

#endif /* _sendMessage_H_ */
