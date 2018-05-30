#include "sendMessage.h"

void Send( char *_number, char *_message )
{
	uint8_t code = 0;
	HAL_TIM_Base_Stop_IT( &htim2 );
	
	CMGS( _number );
	HAL_Delay( 1000 );
	while( ( buffRx[ 2 ] != '>' ) && ( code < 20 ) )
	{
		code++;
		HAL_Delay( 500 );
	};
	MSJ( _message );
	HAL_Delay( 1000 );
	code = 0;
}
