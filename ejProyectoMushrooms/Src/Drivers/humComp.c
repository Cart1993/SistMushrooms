#include "humComp.h"

typedef struct{
  uint16_t humMin;
  uint16_t humMax;
}tHumCompSetPoint;

/*Set point of the minimum and maximum soil's humidity for each phase of cultivation*/
tHumCompSetPoint faseHumCompSetPoint[faseMax] = 
{
  [faseGerminacion]       = { 90 , 100 },
  [fasePreFructificacion] = { 80 , 90 },
  [faseIndFructificacion] = { 85 , 90 },
  [fasePlFructificacion]  = { 75 , 85 },
  [faseResFructificacion] = { 75 , 85 },
};
/*Set point of the minimum and maximum soil's humidity for each phase of cultivation*/
/*Set point de la humedad del sustrato minima y maxima para cada fase de cultivacion*/

/*Function of the control to the soil humidity of cultivation*/
void controlHumComp( eFase x )
{
	uint8_t promHum = ( hum1 + hum2 )/2;	
	
	/*Error inexistent level*/
	if( x>= faseMax) return;
	/*Error inexistent level*/
	/*Error de fase inexistente*/
	
	/*If the humidity of soil is between stablishment ranges*/
	if(( promHum >= faseHumCompSetPoint[x].humMin ) && ( promHum <= faseHumCompSetPoint[x].humMax ))
  {
		if( sensorOptic == 0 )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		}	
  }
	/*If the humidity of soil is between stablishment ranges*/
	/*Si la humedad del sustrato se encuentra entre los rangos establecidos*/
	
	/*If the temperature of soil is out of range. More than maxim range*/
  if( promHum > faseHumCompSetPoint[x].humMax )
  {
		if( ( sensorOptic == 1) || ( sensorOptic == 2 ) )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		}
		if( cntHumComp == desocupado )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_SET );
		}
		else
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		}
  }
	/*If the temperature of soil is out of range. More than maxim range*/
  /*Si la humedad del sustrato supera el umbral de la maxima humedad posible*/
	
	/*If the temperature of soil is under the range*/
  if( promHum < faseHumCompSetPoint[x].humMin )
  {
		if( sensorOptic == 0 )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET );
		}
		else
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET );
		}
		if( cntHumComp == desocupado )
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		}
		else
		{
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_SET );
		}
  }
	/*If the temperature of soil is under the range*/
	/*Si la humedad del compost disminuye del umbral de la minima humedad permitida*/
}
/*Function of the control to the soil humidity of cultivation*/
