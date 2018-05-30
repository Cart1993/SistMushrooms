#include "tempMCU.h"

void tempSensor( void )
{
	tempSensorMCU = buffer[ 5 ];
	tempSensorMCU = ( ( ( tempSensorMCU*( 3300/4096 ) ) - V25 )/( Avg_Slope ) + 25 );
	if( ( tempSensorMCU > -40 ) && ( tempSensorMCU < 0 ) )
	{
		HAL_GPIO_WritePin( GPIOC, GPIO_PIN_11, GPIO_PIN_SET );
		LCD_clear();
		HAL_Delay( 10 );
		disp_enviar( "Temp Baja", 0 );
		HAL_Delay( 10 );
		disp_enviar( "MCU ALERT", 1 );
		HAL_Delay( 10 );
		alertaTempMcuHigh = 2;
	}
	if( ( tempSensorMCU > 45 ) )
	{
		HAL_GPIO_WritePin( GPIOC, GPIO_PIN_11, GPIO_PIN_SET );
		LCD_clear();
		HAL_Delay( 10 );
		disp_enviar( "Temp Elevada", 0 );
		HAL_Delay( 10 );
		disp_enviar( "MCU ALERT", 1 );
		HAL_Delay( 10 );
		process = 6;
		alertaTempMcuHigh = 1;
	}
}
