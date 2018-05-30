#include "confgGPRS.h"

int8_t initGPRS( void )
{
	switch( GPRS )
	{
		case SERVICE:
			retry = 0;
			while( ( retry >= 0 ) && ( retry < 3 ) )
			{
				CGATT( "1" );
				HAL_Delay( 2000 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					GPRS = REGISTRATION;
					retry = -1;
				}
				else
				{
					retry++;
				}
			}
			if( retry == 3 ) return -1;
		case REGISTRATION:
			retry = 0;
			while( ( retry >= 0 ) && ( retry < 3 ) )
			{
				CGREG( "1" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					GPRS = SOCKET;
					retry = -1;
				}
				else
				{
					retry++;
				}
			}
			if( retry == 3 ) return -1;
		case SOCKET:
			retry = 0;
			while( ( retry >= 0 ) && ( retry < 3 ) )
			{
				CSTT2( "gprsweb.digitel.ve", "", "" );
				HAL_Delay( 2000 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					GPRS = WIRELESS;
					retry = -1;
				}
				else
				{
					retry++;
				}
			}
		case WIRELESS:
			retry = 0;
			while( ( retry >= 0 ) && ( retry < 3 ) )
			{
				CIICR();
				HAL_Delay( 2000 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					GPRS = LOCALIP;
					retry = -1;
				}
				else
				{
					retry++;
				}
			}
		case LOCALIP:
			CIFSR();
			HAL_Delay( 2000 );
			GPRS = BEARER;
		case BEARER:
			SAPBRCONFG( "3", "1", "CONTYPE", "GPRS" );
			HAL_Delay( 100 );
			SAPBRCONFG( "3", "1", "APN", "gprsweb.digitel.ve" );
			HAL_Delay( 100 );
			SAPBR( "1", "1" );
			HAL_Delay( 5000 );
			SAPBR( "2", "1" );
			HAL_Delay( 3000 );
			return 0;
		default:
			return -1;
	}
}
