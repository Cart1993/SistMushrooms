#include "getHTTP.h"

int8_t GETHTTP( char *CID, char *URL )
{
	switch( paso )
	{
		case INITGET:
			Module_Write_str( "AT+HTTPINIT" );
			Module_Write_str( "\r\n" );
			HAL_Delay( 10 );
			paso = PARACIDGET;
		case PARACIDGET:
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
				 paso = PARAURLGET;
			 }
		case PARAURLGET:
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
				paso = ACTIONGET;
			}
		case ACTIONGET:
			Module_Write_str( "AT+HTTPACTION=0" );
			Module_Write_str( "\r\n" );
			HAL_Delay( 5000 );
			if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
			 && ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
			{
				return -1;
			}
			else
			{
				if( ( buffRx[ 23 ] == '2' ) && ( buffRx[ 24 ] == '0' ) && ( buffRx[ 25 ] == '0' ) )
				{
					paso = READGET;
				}
				else
				{
					return -1;
				}
			}
		case READGET:
			Module_Write_str( "AT+HTTPREAD" );
			Module_Write_str( "\r\n" );
			HAL_Delay( 5000 );
			if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
			 && ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
			{
				return -1;
			}
			else
			{
				if( buffRx[ 15 ] != NULL )
				{
					for( uint8_t x = 0; x<10 ; x++)
					{
						if( buffRx[ 13 ] == (0x30+x) ) i = x*100;
						if( buffRx[ 14 ] == (0x30+x) ) j = x*10;
						if( buffRx[ 15 ] == (0x30+x) ) k = x;
					}
					i = i + j + k;
					for( uint8_t x = 0; x<i ; x++)
					{
						buffRxRead[ x ] = buffRx[ 18+x ];
					}
				}
				else
				{
					for( uint8_t x = 0; x<10 ; x++)
					{
						if( buffRx[ 13 ] == (0x30+x) ) i = x*10;
						if( buffRx[ 14 ] == (0x30+x) ) j = x;
					}
					i = i + j;	
					for( uint8_t x = 0; x<i ; x++)
					{
						buffRxRead[ x ] = buffRx[ 17+x ];
					}
				}
			}
			paso = TERMGET;
		case TERMGET:
			Module_Write_str( "AT+HTTPTERM" );
			Module_Write_str( "\r\n" );
			HAL_Delay( 100 );
			if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' )
			 && ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
			{
				return -1;
			}
			else
			{
				return 0;
			}
		default:
			return -1;
	}
}
