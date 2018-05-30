#include "tempComp.h"

typedef struct{
  uint16_t temMin;
  uint16_t temMax;
}tTemCompSetPoint;

/*Set point of the minimum and maximum growing's temperature for each phase of cultivation*/
tTemCompSetPoint faseTemCompSetPoint[faseMax] = 
{
  [faseGerminacion]       = { 24 , 28 },
  [fasePreFructificacion] = { 21 , 26 },
  [faseIndFructificacion] = { 17 , 21 },
  [fasePlFructificacion]  = { 17 , 22 },
  [faseResFructificacion] = { 17 , 22 },
};
/*Set point of the minimum and maximum growing's temperature for each phase of cultivation*/
/*Set point de la temperatura del sustrato minima y maxima para cada fase de cultivacion*/

/*Control of the growing's temperature in the cultivation room*/
void controlTempComp( eFase x )
{
	uint16_t promTempComp = tempComp1;
	
	tempComp1 = ReportTemperature();
	
	/*Error inexistent level*/
	if( x >= faseMax ) return;
	/*Error inexistent level*/
	/*Error de fase inexistente*/
	
	/*If the temperature of soil is between stablishment ranges*/
	if(( promTempComp >= faseTemCompSetPoint[x].temMin ) && ( promTempComp <= faseTemCompSetPoint[x].temMax ))
	{
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		cntHumComp = desocupado;
	}
	/*If the temperature of soil is between stablishment ranges*/
	/*Si la temperatura del compost se encuentra entre los rangos establecidos*/
	
	/*If the temperature of soil is out of range. More than maxim range*/
	if( promTempComp >= faseTemCompSetPoint[x].temMax )
	{
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		cntHumComp = desocupado;
		if( sensorOptic == 0 )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		}
		else
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET );
		}
		if( cntAA == desocupado )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_SET );
		}
		else
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_RESET );
		}
	}
	/*If the temperature of soil is out of range. More than maxim range*/
	/*Si la temperatura del compost se encuentra por encima de los rangos requeridos*/
	
	/*If the temperature of soil is under the range*/
	if( ( promTempComp < faseTemCompSetPoint[x].temMin ) && ( promTempComp > 0 ) )
	{
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_SET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		cntHumComp = ocupado;
	}
	/*If the temperature of soil is under the range*/
	/*Si la temperatura del compost disminuye del umbral de la minima temperatura*/
	
}
/*Control of the growing's temperature in the cultivation room*/
