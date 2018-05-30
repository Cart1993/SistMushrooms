#include "confgGSM.h"

int8_t initGSM( void )
{
	switch( caso )
	{
		case format:
			retryTimes = 0;
			while( ( retryTimes >= 0 ) && ( retryTimes < 3 ) )
			{
				CMGF( "1" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					caso = character;
					retryTimes = -1;
				}
				else
				{
					retryTimes++;
				}
			}
			if( retryTimes == 3 ) return 0;
		case character:
			retryTimes = 0;
			while( ( retryTimes >= 0 ) && ( retryTimes < 3 ) )
			{
				CSCS( "GSM" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					caso = erase;
					retryTimes = -1;
				}
				else
				{
					retryTimes++;
				}
			}
			if( retryTimes == 3 ) return 0;
		case erase:
			retryTimes = 0;
			while( ( retryTimes >= 0 ) && ( retryTimes < 3 ) )
			{
				CMGD( "1" , "4" );
				HAL_Delay( 2000 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					retryTimes = -1;
				}
				else
				{
					retryTimes++;
				}
			}
			if( retryTimes == 3 ) return 0;
			return -1;
		default:
			return 0;
	}
}
