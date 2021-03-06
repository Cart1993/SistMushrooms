#include "controlAmbiental.h"

void controlAmbiental( void )
{  
	if( system != OFF )
	{
		/*Control of the illumination at the growing's room*/
		switch( getLight() )
		{
			case LightHigh:
				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET );			// Closed the door
				cntLight = ocupado;
				break;
			case LightMedium:
				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_RESET );		// Open the door
				cntLight = ocupado;
				break;
			case LightLow:
				cntLight = desocupado;
				break;
			default:
				break;
		}
		/*Control of the illumination at the growing's room*/
		/*Control de la iluminacion en la nave de cultivo*/
		
		/*Control of the level tank at the growing's room*/
		switch ( sensorOptic )
		{
			case VOID:
				HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_SET );			// Turn On the admission valve
				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );		// Turn Off the extraction valve
				levelTank = ocupado;
				break;
			case MEDIUM:
				HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_SET );			// Turn On the admission valve
				levelTank = ocupado;
				break;
			case FULL:
				HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_RESET );		//Turn Off the admission valve
				levelTank = desocupado;
				break;
			default:
				break;
		}
		/*Control of the level tank at the growing's room*/
		/*Control del nivel del tanque de agua en la nave de cultivo*/
		
		/*Control of the temperature & humidity of the ambient and humidity & temperature of the soil
		  for each one of the cultivation pahses*/
    controlTempAmb( fase( date ) );
		controlTempComp( fase( date ) );
    controlHumComp( fase( date ) );
    controlHumAmb( fase( date ) );
		tempSensor();
		/*Control of the temperature & humidity of the ambient and humidity & temperature of the soil
		  for each one of the cultivation pahses*/
		/*Control de la temperatura & humedad del ambiente y humedad & temperatura del compost
			para cada uno de las fases de cultivacion*/
	}
	else if( system == OFF )
	{		
		/*SHUTTING DOWN OF THE CONTROLS*/
		HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_RESET );
		/*SHUTTING DOWN OF THE CONTROLS*/
		/*APAGADO DE LOS CONTROLES*/
		
	}
}
