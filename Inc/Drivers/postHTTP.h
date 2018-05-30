
/**
* @file postHTTP.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library method POST HTTP
*	@brief Library to apply the method POST on HTTP
*	@description This library is to apply the method POST from HTTP on SIM800L
*
*/

#ifndef _postHTTP_H_
#define _postHTTP_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion eCasePOST que contiene las etapas a aplicar para implementar el metodo POST sobre HTTP 
*
*/

typedef enum
{
	INITPOST,
	PARACIDPOST,
	PARAURLPOST,
	DATAPOST,
	ACTIONPOST,
	TERMPOST,
}eCasePOST;

/**
*	@brief Variable eCasePost llamada action que inicia en INITPOST, esta establece el flujo de la funcion
*
*/

eCasePOST action = INITPOST;

/**
*	@brief Variable cntData contador para saber cuando inicia y termina el servicio HTTP Post
*
*/

extern uint8_t g_cntData;

/**
*	@brief Variable firstRound valor para indicar que es el 1er intento del servicio HTTP Post
*
*/

extern bool b_firstRound;

/**
*	@brief Array externo de 8 bits sin signo buffRx donde se almacena la data enviada al MCU
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief Function POSTHTTP metodo POST sobre HTTP para el modulo SIM800L
*	@param CID Perfil Bearer
*	@param URL Direccion URL a la que se le aplica el metodo POST
*	@param sizeData Tamaño de la data que se enviara al servidor a traves del metodo POST
*	@param latency Tiempo de intentos de reenvio o repost de la data al servidor
*	@param Data Datos a enviar
*	@return int8_t Entero de 8 bits (0---> Good method, 1---> Bad method)
*
*/

int8_t POSTHTTP( char *CID, char *URL, char *sizeData, char *latency, char *Data );

#endif /* _postHTTP_H_ */
