#include "tempAmb.h"

typedef struct{
  uint16_t temMin;
  uint16_t temMax;
}tTemAmbSetPoint;

/*Set point of the minimum and maximum ambient's temperature for each phase of cultivation*/
tTemAmbSetPoint faseTemAmbSetPoint[faseMax] = 
{
  [faseGerminacion]       = { 18 , 23 },
  [fasePreFructificacion] = { 16 , 20 },
  [faseIndFructificacion] = { 16 , 19 },
  [fasePlFructificacion]  = { 15 , 19 },
  [faseResFructificacion] = { 15 , 19 },
};
/*Set point of the minimum and maximum ambient's temperature for each phase of cultivation*/
/*Set point de la temperatura ambiental minima y maxima para cada fase de cultivacion*/

/*Control of the ambient's temperature in the cultivation room*/
void controlTempAmb( eFase x )
{	
	DHT11( GPIO_PIN_6 );
	read( &humidity1, &humidity2, &temperature1, &temperature2 );
	HAL_Delay( 200 );
	DHT11( GPIO_PIN_8 );
	read( &humidity1, &humidity2, &temperature11, &temperature2 );
	HAL_Delay( 200 );
	
	temperature1 = ( temperature1 + temperature11 ) / 2;
	
  /*Error inexistent level*/
  if( x >= faseMax ) return;
	/*Error inexistent level*/
	/*Error de fase inexistente*/

	/*If the temperature of the ambient is between stablishment ranges*/
  if( ( temperature1 >= faseTemAmbSetPoint[x].temMin ) && ( temperature1 <= faseTemAmbSetPoint[x].temMax ) )
  {
		countTemp = desocupado;
		alertaTempHigh = desocupado;
		alertaTempLow = desocupado;
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_SET );
		cntAA = desocupado;
		cntHum = desocupado;
  }
	/*If the temperature of the ambient is between stablishment ranges*/
	/*Si la temperatura ambiente se encuentra entre los rangos establecidos*/
  
	/*If the temperature of the ambient is out of range. More than the maxim range*/
  if( temperature1 > faseTemAmbSetPoint[x].temMax )
  {
    HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_SET );
		if( cntLight != ocupado ) HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET );
		cntHum = ocupado;
		countTemp++;
		if( countTemp > timeout )
		{
			alertaTempHigh = ocupado;
			CMGS( "+584243458044" );
			countTemp = desocupado;
		}
  }
	/*If the temperature of the ambient is out of range. More than the maxim range*/
  /*Si la temperatura ambiente supera el umbral de la maxima temperatura*/
	
	/*If the temperature of soil is under the range*/
  if( temperature1 < faseTemAmbSetPoint[x].temMin )
  {
    HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET );
		cntAA = ocupado;
		cntHum = desocupado;
		countTemp++;
		if( countTemp > timeout )
		{
			alertaTempLow = ocupado;
			CMGS( "+584243458044" );
			countTemp = desocupado;
		}
  }
	/*If the temperature of soil is under the range*/
	/*Si la temperatura ambiente disminuye del umbral de la minima temperatura*/
}
/*Control of the growing's temperature in the cultivation room*/
