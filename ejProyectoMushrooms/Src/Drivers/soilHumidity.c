#include "soilHumidity.h"

const uint8_t humedadBaja = 25;
const uint8_t humedadMedia = 50;
const uint8_t humedadMediaAlta = 75;

eHum getSoilHumidity( void ){
	
	hum1 = ( 100-(( 100.0/255.0 )*(buffer[3] )));
	hum2 = ( 100-(( 100.0/255.0 )*(buffer[4] )));
	humTotal = (hum1+hum2)/2;
	
	if(( ( humTotal>0 ) && ( humTotal<humedadBaja ) ))
	{
		return humLow;
	}
	else if(( ( humTotal>=humedadBaja ) && ( humTotal<humedadMedia ) ))
	{
		return humMedium;
	}
	else if(( ( humTotal>=humedadMedia ) && ( humTotal<humedadMediaAlta ) ))
	{
		return humMediumHigh;
	}
	else if( ( humTotal>=humedadMediaAlta ) || ( humTotal>=humedadMediaAlta ) )
	{
		return humHigh;
	}
	else
	{
		return humError;
	}
}
