#include "humAmb.h"

typedef struct{
  uint16_t humMin;
  uint16_t humMax;
}tHumAmbSetPoint;


/*Set point of the minimum and maximum ambient's humidity for each phase of cultivation*/
tHumAmbSetPoint faseHumAmbSetPoint[faseMax] = 
{
  [faseGerminacion]       = { 94 , 100 },
  [fasePreFructificacion] = { 87 , 93 },
  [faseIndFructificacion] = { 87 , 90 },
  [fasePlFructificacion]  = { 82 , 87 },
  [faseResFructificacion] = { 75 , 85 },
};
/*Set point of the minimum and maximum ambient's humidity for each phase of cultivation*/

/*Control of the ambient humidity in the cultivation room*/
void controlHumAmb( eFase x )
{
	
	/*Error inexistent level*/
  if( x >= faseMax ) return;
	/*Error inexistent level*/
	/*Error de fase inexistente*/
	
	/*If the ambient humidity is between stablishment ranges*/
  if(( humidity1 >= faseHumAmbSetPoint[x].humMin ) && ( humidity1 <= faseHumAmbSetPoint[x].humMax ))
  {
		countHum = desocupado;
		alertaHumLow = desocupado;
		alertaHumHigh = desocupado;
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, GPIO_PIN_RESET );
		if( cntHum == desocupado )
		{
			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_RESET );
		}
  }
	/*If the ambient humidity is between stablishment ranges*/
	/*Si la humedad ambiental se encuentra entre los rangos establecidos*/
	
	/*If the soil humidity is out of range. More than maxim range*/
  if( humidity1 > faseHumAmbSetPoint[x].humMax )
  {
		HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, GPIO_PIN_RESET );
		if( cntHum == desocupado )
		{
			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET );
		}
		countHum++;
		if( countHum >= timeout )
		{
			alertaHumHigh = ocupado;
			CMGS( "+584243458044" );
		}
  }
	/*If the soil humidity is out of range. More than maxim range*/
  /*Si la humedad del ambiente supera el umbral de la maxima humedad soportada*/
	
	/*If the humidity of soil is under the range*/
  if( humidity1 < faseHumAmbSetPoint[x].humMin )
  {
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, GPIO_PIN_SET );
		if( cntHum == desocupado )
		{
			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_RESET );
		}
		countHum++;
		if( countHum >= timeout )
		{
			alertaHumLow = ocupado;
			CMGS( "+584243458044" );
		}
  }
	/*If the humidity of soil is under the range*/
	/*Si la humedad ambiental disminuye del umbral de la minima requerida*/
}
/*Control of the ambient humidity in the cultivation room*/
