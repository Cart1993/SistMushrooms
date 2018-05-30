#include "funcionInicializacion.h"

int8_t Inicializacion( void )
{
	switch( ctrl )
	{
		case EchoMode:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				Cmd_ATE( "0" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					ctrl = connection;
					intento = -1;
				}
				else
				{
					intento++;
				}
			}
			if( intento == 3 ) return -1;
		case connection:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				Cmd_AT();
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					ctrl = BaudRate;
					intento = -1;
				}
				else
				{
					intento++;
				}
			}
			if( intento == 3 ) return -1;
		case BaudRate:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				IPR( "115200" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					intento = 0;
					while( ( intento >= 0 ) && ( intento < 3 ) )
					{
						IPR( "?" );
						HAL_Delay( 10 );
						if( ( buffRx[ 8 ] == '1' ) && ( buffRx[ 9 ] == '1' ) && ( buffRx[ 10 ] == '5' ) && ( buffRx[ 11 ] == '2' ) 
						&& ( buffRx[ 12 ] == '0' ) && ( buffRx[ 13 ] == '0' ) && ( buffRx[ 18 ] == 'O' ) && ( buffRx[ 19 ] == 'K' ) )
						{
							ctrl = TempSim;
							intento = -1;
						}
						else
						{
							intento++;
						}
					}	
				}
				else
				{
					intento++;
				}
			}
			if( intento >= 3 ) return -1;
		case TempSim:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				CMTE( "1" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					intento = 0;
					while( ( intento >= 0 ) && ( intento < 3 ) )
					{
						CMTE( "?" );
						HAL_Delay( 1000 );
						if( ( buffRx[ 11 ] > 0x30 ) || ( buffRx[ 11 ] < 0x34 ) )
						{
							ctrl = functionality;
							intento = -1;
						}
						else
						{
							intento++;
						}
					}
				}
				else
				{
					intento++;
				}
			}
			if( intento >= 3 ) return -1;
		case functionality:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				CFUN( "1" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					intento = 0;
					while( ( intento >= 0 ) && ( intento < 3 ) )
					{
						CFUN( "?" );
						HAL_Delay( 1000 );
						if( ( buffRx[ 9 ] == '1' ) && ( buffRx[ 14 ] == 'O' ) && ( buffRx[ 15 ] == 'K' ) )
						{
							ctrl = registration;
							intento = -1;
						}
						else
						{
							intento++;
						}
					}
				}
				else
				{
					intento++;
				}
			}
			if( intento >= 3 ) return-1;	
		case registration:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				CREG( "1" );
				HAL_Delay( 10 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					intento = 0;
					while( ( intento >= 0 ) && ( intento < 3 ) )
					{
						CREG( "?" );
						HAL_Delay( 100 );
						if( ( buffRx[ 9 ] == '1' ) && ( buffRx[ 11 ] == '1' ) && ( buffRx[ 16 ] == 'O' ) && ( buffRx[ 17 ] == 'K' ) )
						{
							ctrl = operator;
							intento = -1;
						}
						else
						{
							intento++;
						}
					}
				}
				else
				{
					intento++;
				}
			}
			if( intento >= 3 ) return -1;
		case operator:
			intento = 0;
			while( ( intento >= 0 ) && ( intento < 3 ) )
			{
				COPS( "0" );
				HAL_Delay( 100 );
				if( ( buffRx[ 2 ] == 'O' ) && ( buffRx[ 3 ] == 'K' ) )
				{
					intento = 0;
					while( ( intento >= 0 ) && ( intento < 3 ) )
					{
						intento = 0;
						COPS( "?" );
						HAL_Delay( 1000 );
						if( ( buffRx[ 14 ] == 'D' ) && ( buffRx[ 15 ] == 'I' ) && ( buffRx[ 16 ] == 'G' ) && ( buffRx[ 17 ] == 'I' )
						&& ( buffRx[ 18 ] == 'T' ) && ( buffRx[ 19 ] == 'E' ) && ( buffRx[ 20 ] == 'L' ) && ( buffRx[ 22 ] == 'T' )
						&& ( buffRx[ 23 ] == 'I' ) && ( buffRx[ 24 ] == 'M' ) && ( buffRx[ 30 ] == 'O' ) && ( buffRx[ 31 ] == 'K' ) )
						{
							intento = -1;
							break;
						}
						else
						{
						intento++;
						}
					}
				}
				else
				{
					intento++;
				}
			}
			if( intento >= 3 ) return -1;
			break;
		default:
			return -1;
	}
	return 0;
}
