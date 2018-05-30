/**
 ******************************************************************************
 * @file    Fibocom.c
 * @author  TFHKA
 * @version V1.0.1
 * @brief   Contiene el desarrollo de drivers para el manejo del modulo Fibocom
 ******************************************************************************
 */

#include "Fibocom.h"
/**/
#include "stm32f4xx_hal.h"
/**/
#include <string.h>
#include "AT.h"

#define PWON_PIN                          GPIO_PIN_6
#define PWON_GPIO_PORT                    GPIOH
#define PWON_GPIO_CLK_ENABLE()            __GPIOH_CLK_ENABLE()
  
#define ONLED_PIN                         GPIO_PIN_7
#define ONLED_GPIO_PORT                   GPIOH
#define ONLED_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()

#define RESET_PIN                         GPIO_PIN_8
#define RESET_GPIO_PORT                   GPIOH
#define RESET_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()

extern TIM_HandleTypeDef htim2;

typedef enum
{
	MEDICION,
	CONTROL,
	DATOS,
	SMSRX,
	SMSSEND,
	ALERTA,
	OUT,
	CALL,
}eProcess;

extern bool msg;

extern uint8_t process, call, g_errorCount;

extern int8_t receiveMsj;

extern uint8_t buffRx[ 2000 ];

extern UART_HandleTypeDef huart6;

static uint8_t Data;

/**
 * @brief  Configuracion de lineas de control de fibocom
 * @param  None
 * @retval None
 */
uint8_t fibocom_bsp_init( void )
{
  
  HAL_UART_Receive_IT( &huart6 , &Data , 1 );
  
  return true;
}


/* Enabling the Module*/
void fibocom_set_pwon( bool bEstado )
{
	
}

bool fibocom_Get_estado()
{
	
  return true;
}

void fibocom_on( void )
{
	
}

void fibocom_off( void )
{
	
}

void fibocom_reset( void )
{
	
}

bool Module_Write_str( char *str )
{
    while( HAL_UART_Transmit_IT( &huart6, (uint8_t*)str , strlen(str) ) == HAL_BUSY)
    {
    }
    
		for( uint16_t x=0 ; x<300; x++)
		{
			buffRx[ x ] = 0x00;
		}
    return true;
}

bool Module_Write(uint8_t* Data,uint16_t iLen)
{
    HAL_UART_Transmit_IT( &huart6, Data , iLen );
    return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	int8_t i = 0;
	  
  HAL_UART_Receive_IT( &huart6 , &Data , 1 );
	
	HAL_TIM_Base_Stop_IT( &htim2 );
	
	while( i >= 0 )
	{
		if( buffRx[ i ] == NULL )
		{
			buffRx[ i ] = Data;
			i = -1;
		} 
		else
		{
			i++;
		}
	}
	
	//Verify that the new message has been received
	if( ( buffRx[ 2 ] == '+' ) && ( buffRx[ 3 ] == 'C' ) && ( buffRx[ 4 ] == 'M' ) && ( buffRx[ 5 ] == 'T' ) 
	&& ( buffRx[ 6 ] == 'I' ) && ( buffRx[ 7 ] == ':' ) && ( buffRx[ 10 ] == 'S' ) && ( buffRx[ 11 ] == 'M' ) 
	&& ( buffRx[ 14 ] == '1' ) )
	{
		msg = true;
		process = SMSRX;
		g_errorCount = 0;
	}
	//Verify that the message has been send succesfully
	else if( ( ( buffRx[ 2 ] == '+' ) && ( buffRx[ 3 ] == 'C' ) && ( buffRx[ 4 ] == 'M' ) && ( buffRx[ 5 ] == 'G' )
		&& ( buffRx[ 6 ] == 'S' ) && ( buffRx[ 16 ] == 'O' ) && ( buffRx[ 17 ] == 'K' ) ) || ( ( buffRx[ 2 ] == '+' )
		&& ( buffRx[ 3 ] == 'C' ) && ( buffRx[ 4 ] == 'M' ) && ( buffRx[ 5 ] == 'G' )
		&& ( buffRx[ 6 ] == 'S' ) && ( buffRx[ 15 ] == 'O' ) && ( buffRx[ 16 ] == 'K' ) ) )
	{
		process = SMSSEND;
		g_errorCount = 0;
	}
	//Answer the incoming call to hang up
	else if( ( ( buffRx[ 2 ] == 'R' ) && ( buffRx[ 3 ] == 'I' ) && ( buffRx[ 4 ] == 'N' ) && ( buffRx[ 5 ] == 'G' ) ) ||
				( ( buffRx[ 8 ] == 'R' ) && ( buffRx[ 9 ] == 'I' ) && ( buffRx[ 10 ] == 'N' ) && ( buffRx[ 11 ] == 'G' ) ) ||
				( ( buffRx[ 14 ] == 'R' ) && ( buffRx[ 15 ] == 'I' ) && ( buffRx[ 16 ] == 'N' ) && ( buffRx[ 17 ] == 'G' ) ) ||
				( ( buffRx[ 9 ] == 'R' ) && ( buffRx[ 10 ] == 'I' ) && ( buffRx[ 11 ] == 'N' ) && ( buffRx[ 12 ] == 'G' ) )	)
	{
		call++;
		if( call == 4 )
		{
			process = CALL;
			g_errorCount = 0;
		}
	}
	//The system check for Errors in the Module SIM800 through Result Code 
//	else if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' ) && ( buffRx[ 5 ] == 'O' )
//		&& ( buffRx[ 6 ] == 'R' ) )
//	{
//		g_errorCount++;
//		for( uint8_t x=0 ; x<100; x++)
//		{
//			buffRx[ x ] = 0x00;
//		}
//		Cmd_ATQ( "1" );
//		HAL_Delay( 10 );
//	}
	else
	{
		process = MEDICION;
	}
	
}
