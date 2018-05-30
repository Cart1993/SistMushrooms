#include "sendMessage.h"

void Send( char *_number, char *_message )
{
	CMGS( _number );
	HAL_Delay( 1000 );
	while( buffRx[ 2 ] != '>' );
	MSJ( _message );
	HAL_Delay( 1000 );
}
