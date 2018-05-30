#include "tempMCU.h"

void tempSensor( void )
{
	tempSensorMCU = buffer[ 5 ];
	tempSensorMCU = ( ( ( tempSensorMCU*( 3300/4096 ) ) - V25 )/( Avg_Slope ) + 25 );
	
	if( ( tempSensorMCU > -40 ) && ( tempSensorMCU < 0 ) )
	{
		g_countMcu++;
		if( g_countMcu >= timeout )
		{
		alertaTempMcuLow = ocupado;
		}
	}
	else if( ( tempSensorMCU > 45 ) )
	{
		g_countMcu++;
		if( g_countMcu >= timeout )
		{
			alertaTempMcuHigh = ocupado;
		}
	}
	else
	{
		g_countMcu = 0;
		alertaTempMcuHigh = desocupado;
		alertaTempMcuLow = desocupado;
	}
}
