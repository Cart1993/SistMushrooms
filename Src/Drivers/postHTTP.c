#include "postHTTP.h"

int8_t POSTHTTP( char *CID, char *URL, char *sizeData, char *latency, char *Data )
{
	switch( action )
	{
		case INITPOST:
			if( ( 0 == g_cntData ) && ( false == b_firstRound ) )
			{
				Module_Write_str( "AT+HTTPINIT" );
				Module_Write_str( "\r\n" );
				HAL_Delay( 10 );
				action = PARACIDPOST;
			}
			else
			{
				action = PARACIDPOST;
			}
		case PARACIDPOST:
			if( ( 0 == g_cntData ) && ( false == b_firstRound ) )
			{
				Module_Write_str( "AT+HTTPPARA=\"" );
				Module_Write_str( "CID" );
				Module_Write_str( "\"" );
				Module_Write_str( "," );
				Module_Write_str( CID );
				Module_Write_str( "\r\n" );
				HAL_Delay( 100 );
				if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
				&& ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
				{
					return -1;
				}
				else
				{
					action = PARAURLPOST;
				}
			}
			else
			{
				action = PARAURLPOST;
			}
		case PARAURLPOST:
			if( ( 0 == g_cntData ) && ( false == b_firstRound ) )
			{
				Module_Write_str( "AT+HTTPPARA=\"" );
				Module_Write_str( "URL" );
				Module_Write_str( "\",\"" );
				Module_Write_str( URL );
				Module_Write_str( "\"" );
				Module_Write_str( "\r\n" );
				HAL_Delay( 2000 );
				if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
				&& ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
				{
					return -1;
				}
				else
				{
					action = DATAPOST;
				}
			}
			else
			{
				action = DATAPOST;
			}
		case DATAPOST:
			Module_Write_str( "AT+HTTPDATA=" );
			Module_Write_str( sizeData );
			Module_Write_str( "," );
			Module_Write_str( latency );
			Module_Write_str( "\r\n" );
			HAL_Delay( 1000 );
			if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
			 && ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
			{
				return -1;
			}
			else
			{
				Module_Write_str( Data );
				Module_Write_str( "\r\n" );
				HAL_Delay( 5000 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					action = ACTIONPOST;
				}
				else
				{
					return -1;
				}
			}
		case ACTIONPOST:
			Module_Write_str( "AT+HTTPACTION=1" );
			Module_Write_str( "\r\n" );
			HAL_Delay( 5000 );
			if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
			{
				if( ( buffRx[ 23 ] == '2' ) && ( buffRx[ 24 ] == '0' ) && ( buffRx[ 25 ] == '0' ) )
				{
					action = TERMPOST;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		case TERMPOST:
			if( 6 == g_cntData )
			{ 
				Module_Write_str( "AT+HTTPTERM" );
				Module_Write_str( "\r\n" );
				HAL_Delay( 100 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				action = INITPOST;
				return 0; 
			}
		default:
			return -1;
	}
}
