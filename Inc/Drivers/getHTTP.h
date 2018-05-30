
/**
* @file getHTTP.h
* @version 1.0
* @date 22/11/2017
* @author Christian Alberto Rangel Torres
*	@title Library method GET HTTP
*	@brief Library to apply the method GET on HTTP
*	@description This library is to apply the method GET from HTTP on SIM800L
*
*/

#ifndef _getHTTP_H_
#define _getHTTP_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fibocom.h"
#include "AT.h"

/**
*	@brief Estructura de enumeracion eCaseGET que contiene las etapas a aplicar para implementar el metodo GET sobre HTTP 
*
*/

typedef enum
{
	INITGET,
	PARACIDGET,
	PARAURLGET,
	ACTIONGET,
	READGET,
	TERMGET,
}eCaseGET;

/**
*	@brief Variable eCaseGet llamada paso que inicia en INITGET, esta establece el flujo de la funcion
*
*/

eCaseGET paso = INITGET;

/**
*	@brief Variables enteras de 8 bits sin signo i, j, k que sirven para leer y almacenar lo enviado por servidor a traves del metodo GET en el buffer
*
*/

uint8_t i, j, k;

/**
*	@brief Function Module_Write_str enviar string a traves del UART del MCU hacia el modulo SIM800L
*	@param str String de datos a enviar a traves del UART
*	@return bool Valor booleano de envio satisfactorio o insatisfactorio
*
*/

extern bool Module_Write_str( char *str );

/**
*	@brief Array externo de 8 bits sin signo buffRx donde se almacena la data enviada al MCU
*
*/

extern uint8_t buffRx[ 2000 ];

/**
*	@brief Array externo de sombra de 8 bits sin signo buffRxRead donde se almacena la data enviada al MCU
*
*/

extern uint8_t buffRxRead[ 500 ];

/**
*	@brief Function GETHTTP metodo GET sobre HTTP para el modulo SIM800L
*	@param CID Perfil Bearer
*	@param URL Direccion URL a la que se le aplica el metodo GET
*	@return int8_t Entero de 8 bits (0---> Good method, 1---> Bad method)
*
*/

int8_t GETHTTP( char *CID, char *URL );


#endif /* _getHTTP_H_ */
