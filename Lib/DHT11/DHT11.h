/**
*	@file DHT11.h
* @version 1.0
*	@date 19/10/2017
* @author Christian Rangel
*	@code
	int main
	{
		DHT11( GPIO_PIN_8 );
			switch ( i )
			{
				case 0:
					HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, GPIO_PIN_RESET );
					break;
				case 1:
					HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, GPIO_PIN_SET );
					break;
				default:
					break;
			}
			read( &humidity1,&humidity2 ,&temperature1 ,&temperature2 );
			HAL_Delay( 1000 );
	
			DHT11( GPIO_PIN_10 );
			read( &humidity1,&humidity2 ,&temperature1 ,&temperature2 );
			HAL_Delay( 1500 );
	}
*	@endcode
*/

#ifndef DHT11_H_
#define DHT11_H_

#include<stdint.h>

/** 
* @brief	Retardo de reintento de conexion DHT11
*/ 
#define DHT11_RETRY_DELAY 1000  // 1000ms

/**
*	@brief Estructura de enumeracion para entrada y salida
*
*/
typedef enum
{
	INPUT,
	OUTPUT,
} eMode;

/**
*	@brief Estructura de enumeracion para alto y bajo
*
*/
typedef enum
{
	LOW,
	HIGH,
} eEstado;

/**
*	@brief DHT11 Realiza toda la configuracion para la conexion con el sensor DHT11
*	@param pin_number Pin seleccionado para conectar el sensor con el MCU
*	@return void Vacio
*
*/
void DHT11( uint16_t pin_number ) ;

/**
*	@brief read Realiza la lectura de los datos provenientes del sensor DHT11
*	@param humidity1 Puntero que contiene la direccion en donde se aloja la informacion entera de la humedad
*	@param humidity2 Puntero que contiene la direccion en donde se aloja la informacion decimal de la humedad
*	@param temperature1 Puntero que contiene la direccion en donde se aloja la informacion entera de la temperatura
*	@param temperature2 Puntero que contiene la direccion en donde se aloja la informacion decimal de la temperatura
*	@return int8_t	Valor entero del estado de la lectura
*
*/
int8_t read( uint8_t *humidity1,uint8_t *humidity2 , uint8_t *temperature1 , uint8_t *temperature2 ) ;

/**
*	@brief pinMode Funcion para configurar el modo del pin
*	@param pin Pin selecionado a configurar
*	@param mode Modo seleccionado a trabajar con el pin
*	@return int8_t Valor entero del estado de la lectura
*
*/
void pinMode( uint16_t pin , eMode mode );

/**
*	@brief digitalWrite Funcion para escribir en los pines seleccionados
*	@param pin Pin selecionado a configurar
*	@param estado Estado seleccionado a escribir en el pin
*	@return void Vacio
*
*/
void digitalWrite( uint16_t pin, eEstado estado );

/**
*	@brief digitalRead Funcion para escribir en los pines seleccionados
*	@param pin Pin selecionado a configurar
*	@return eEstado	Estado en el que se encuentra el pin seleccionado
*
*/
eEstado digitalRead( uint16_t pin );

#endif /* DHT11_H_ */
