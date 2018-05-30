#include "responseSMS.h"

void SMSquery( uint8_t receiveMsj )
{
switch( receiveMsj )
		{
			case fail:
				break;
			case smsRX:
				CMGR( "1" );
				HAL_Delay( 2000 );
				numeroTelefonico[ 4 ] = buffRx[ 25 ];
				numeroTelefonico[ 5 ] = buffRx[ 26 ];
				numeroTelefonico[ 6 ] = buffRx[ 27 ];
				numeroTelefonico[ 7 ] = buffRx[ 28 ];
				numeroTelefonico[ 8 ] = buffRx[ 29 ];
				numeroTelefonico[ 9 ] = buffRx[ 30 ];
				numeroTelefonico[ 10 ] = buffRx[ 31 ];
				numeroTelefonico[ 11 ] = buffRx[ 32 ];
				numeroTelefonico[ 12 ] = buffRx[ 33 ];
				if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'T' ) && ( buffRx[ 64 ] == 'E')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == 'P') && ( buffRx[ 67 ] == ' ' ) && ( buffRx[ 68 ] == 'A')
					&& ( buffRx[ 69 ] == 'M' ) && ( buffRx[ 70 ] == 'B') && ( buffRx[ 71 ] == ' ' ) && ( buffRx[ 72 ] == 'M')
					&& ( buffRx[ 73 ] == 'U' ) && ( buffRx[ 74 ] == 'S') && ( buffRx[ 75 ] == 'H' ) && ( buffRx[ 76 ] == 'R')
					&& ( buffRx[ 77 ] == 'O' ) && ( buffRx[ 78 ] == 'O') && ( buffRx[ 79 ] == 'M' ) && ( buffRx[ 80 ] == 'S')
					&& ( buffRx[ 85 ] == 'O' ) && ( buffRx[ 86 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, temperaturaAmbiental );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'H' ) && ( buffRx[ 64 ] == 'U')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == ' ') && ( buffRx[ 67 ] == 'A' ) && ( buffRx[ 68 ] == 'M')
					&& ( buffRx[ 69 ] == 'B' ) && ( buffRx[ 70 ] == ' ') && ( buffRx[ 71 ] == 'M' ) && ( buffRx[ 72 ] == 'U')
					&& ( buffRx[ 73 ] == 'S' ) && ( buffRx[ 74 ] == 'H') && ( buffRx[ 75 ] == 'R' ) && ( buffRx[ 76 ] == 'O')
					&& ( buffRx[ 77 ] == 'O' ) && ( buffRx[ 78 ] == 'M') && ( buffRx[ 79 ] == 'S' ) && ( buffRx[ 84 ] == 'O' )
					&& ( buffRx[ 85 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, humedadAmbiental );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'T' ) && ( buffRx[ 64 ] == 'E')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == 'P') && ( buffRx[ 67 ] == ' ' ) && ( buffRx[ 68 ] == 'S')
					&& ( buffRx[ 69 ] == 'U' ) && ( buffRx[ 70 ] == 'S') && ( buffRx[ 71 ] == 'T' ) && ( buffRx[ 72 ] == ' ')
					&& ( buffRx[ 73 ] == 'M' ) && ( buffRx[ 74 ] == 'U') && ( buffRx[ 75 ] == 'S' ) && ( buffRx[ 76 ] == 'H')
					&& ( buffRx[ 77 ] == 'R' ) && ( buffRx[ 78 ] == 'O') && ( buffRx[ 79 ] == 'O' ) && ( buffRx[ 80 ] == 'M')
					&& ( buffRx[ 81 ] == 'S') && ( buffRx[ 86 ] == 'O' ) && ( buffRx[ 87 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, temperaturaSustrato );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'H' ) && ( buffRx[ 64 ] == 'U')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == ' ') && ( buffRx[ 67 ] == 'S' ) && ( buffRx[ 68 ] == 'U')
					&& ( buffRx[ 69 ] == 'S' ) && ( buffRx[ 70 ] == 'T') && ( buffRx[ 71 ] == ' ' ) && ( buffRx[ 72 ] == 'M')
					&& ( buffRx[ 73 ] == 'U' ) && ( buffRx[ 74 ] == 'S') && ( buffRx[ 75 ] == 'H' ) && ( buffRx[ 76 ] == 'R')
					&& ( buffRx[ 77 ] == 'O' ) && ( buffRx[ 78 ] == 'O') && ( buffRx[ 79 ] == 'M' ) && ( buffRx[ 80 ] == 'S' )
					&& ( buffRx[ 85 ] == 'O' ) && ( buffRx[ 86 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, humedadSustrato );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'I' ) && ( buffRx[ 64 ] == 'L')
					&& ( buffRx[ 65 ] == 'U' ) && ( buffRx[ 66 ] == 'M') && ( buffRx[ 67 ] == ' ' ) && ( buffRx[ 68 ] == 'M')
					&& ( buffRx[ 69 ] == 'U' ) && ( buffRx[ 70 ] == 'S') && ( buffRx[ 71 ] == 'H' ) && ( buffRx[ 72 ] == 'R')
					&& ( buffRx[ 73 ] == 'O' ) && ( buffRx[ 74 ] == 'O') && ( buffRx[ 75 ] == 'M' ) && ( buffRx[ 76 ] == 'S')
					&& ( buffRx[ 81 ] == 'O' ) && ( buffRx[ 82 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, iluminacion );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'L' ) && ( buffRx[ 64 ] == 'V')
					&& ( buffRx[ 65 ] == 'L' ) && ( buffRx[ 66 ] == ' ') && ( buffRx[ 67 ] == 'T' ) && ( buffRx[ 68 ] == 'A')
					&& ( buffRx[ 69 ] == 'N' ) && ( buffRx[ 70 ] == 'K') && ( buffRx[ 71 ] == ' ' ) && ( buffRx[ 72 ] == 'M')
					&& ( buffRx[ 73 ] == 'U' ) && ( buffRx[ 74 ] == 'S') && ( buffRx[ 75 ] == 'H' ) && ( buffRx[ 76 ] == 'R')
					&& ( buffRx[ 77 ] == 'O' ) && ( buffRx[ 78 ] == 'O') && ( buffRx[ 79 ] == 'M' ) && ( buffRx[ 80 ] == 'S' )
					&& ( buffRx[ 85 ] == 'O' ) && ( buffRx[ 86 ] == 'K') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, nivelTanque );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'T' ) && ( buffRx[ 64 ] == 'E')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == 'P') && ( buffRx[ 67 ] == ' ' ) && ( buffRx[ 68 ] == 'S')
					&& ( buffRx[ 69 ] == 'I' ) && ( buffRx[ 70 ] == 'M') && ( buffRx[ 71 ] == '8' ) && ( buffRx[ 72 ] == '0')
					&& ( buffRx[ 73 ] == '0' ) )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, temperaturaSim800 );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'T' ) && ( buffRx[ 64 ] == 'E')
					&& ( buffRx[ 65 ] == 'M' ) && ( buffRx[ 66 ] == 'P') && ( buffRx[ 67 ] == ' ' ) && ( buffRx[ 68 ] == 'M')
					&& ( buffRx[ 69 ] == 'C' ) && ( buffRx[ 70 ] == 'U') )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, temperaturaMCU );
				}
				else if( ( buffRx[ 14 ] == 'U' ) && ( buffRx[ 15 ] == 'N') && ( buffRx[ 16 ] == 'R' ) && ( buffRx[ 17 ] == 'E')
					&& ( buffRx[ 18 ] == 'A' ) && ( buffRx[ 19 ] == 'D') && ( buffRx[ 63 ] == 'A' ) && ( buffRx[ 64 ] == 'L')
					&& ( buffRx[ 65 ] == 'L' ) && ( buffRx[ 66 ] == ' ') && ( buffRx[ 67 ] == 'V' ) && ( buffRx[ 68 ] == 'A')
					&& ( buffRx[ 69 ] == 'R' ) && ( buffRx[ 70 ] == 'I') && ( buffRx[ 71 ] == 'A' ) && ( buffRx[ 72 ] == 'B')
					&& ( buffRx[ 73 ] == 'L' ) && ( buffRx[ 74 ] == 'E') && ( buffRx[ 75 ] == 'S' ) && ( buffRx[ 80 ] == 'O')
					&& ( buffRx[ 81 ] == 'K' ) )
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, msjAllVariables );
				}
				else
				{
					CMGD( "1" , "4" );
					HAL_Delay( 2000 );
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					Send( numeroTelefonico, errorMsj );
				}
				break;
				
			case smsTX:
				if( ( ( alertaTempMcuHigh == 1 ) || ( alertaTempMcuLow == 1 ) || ( levelTank == 1 ) || ( lightHigh == 1 ) ||
					( alertaTempHigh == 1 ) || ( alertaTempLow == 1 ) || ( alertaHumLow == 1 )
					|| ( alertaHumHigh == 1 ) ) && ( ( g_countLight > timeout ) || ( g_countTank > timeout ) ||
					( g_countMcu > timeout ) || ( countTemp > timeout ) || ( countHum > timeout ) ) )
				{
					if( alertaTank > 1 )
					{
						CMGD( "1" , "4" );
						HAL_Delay( 2000 );
						for( uint8_t x=0 ; x<100; x++)
						{
							buffRx[ x ] = 0x00;
						}
						Send( numeroTelefonicoAlertas, alertaGeneral );
						alertaTank = 0;
						break;
					}
					else if( ( ( g_countTank > timeout ) || ( levelTank == 1 ) ) && ( alertaTank == 1 ) )
					{
						alertaTank++;
					}
					else
					{
						CMGD( "1" , "4" );
						HAL_Delay( 2000 );
						for( uint16_t x = 0 ; x<400; x++ )
						{
							buffRx[ x ] = 0x00;
						}
						Send( numeroTelefonicoAlertas, alertaGeneral );
						break;
					}
				}
//				if( ( ( alertaTempHigh == 1 ) || ( alertaTempLow == 1 ) ) && ( countTemp > timeout ) )
//				{
//					CMGD( "1" , "4" );
//					HAL_Delay( 2000 );
//					for( uint8_t x=0 ; x<100; x++)
//					{
//						buffRx[ x ] = 0x00;
//					}
//					Send( numeroTelefonicoAlertas, alertaGeneral );
//					break;
//				}
//				for( uint8_t x=0 ; x<100; x++)
//				{
//					buffRx[ x ] = 0x00;
//				}
//				HAL_Delay( 3000 );
//				if( ( lightHigh == 1 ) && ( g_countLight > timeout ) )
//				{
//					CMGD( "1" , "4" );
//					HAL_Delay( 2000 );
//					for( uint8_t x=0 ; x<100; x++)
//					{
//						buffRx[ x ] = 0x00;
//					}
//					Send( numeroTelefonicoAlertas, alertaGeneral );
//					break;
//				}
//				for( uint8_t x=0 ; x<100; x++)
//				{
//					buffRx[ x ] = 0x00;
//				}
//				HAL_Delay( 3000 );
//				if( ( levelTank == 1 ) && ( g_countTank > timeout ) )
//				{
//					if( alertaTank > 1 )
//					{
//						CMGD( "1" , "4" );
//						HAL_Delay( 2000 );
//						for( uint8_t x=0 ; x<100; x++)
//						{
//							buffRx[ x ] = 0x00;
//						}
//						Send( numeroTelefonicoAlertas, alertaGeneral );
//						alertaTank = 0;
//						break;
//					}
//					else
//					{
//						alertaTank++;
//					}
//				}
//				for( uint8_t x=0 ; x<100; x++)
//				{
//					buffRx[ x ] = 0x00;
//				}
//				HAL_Delay( 3000 );
//				if( ( ( alertaTempMcuHigh == 1 ) || ( alertaTempMcuLow == 1 ) ) && ( g_countMcu > timeout ) )
//				{
//					CMGD( "1" , "4" );
//					HAL_Delay( 2000 );
//					for( uint8_t x=0 ; x<100; x++)
//					{
//						buffRx[ x ] = 0x00;
//					}
//					Send( numeroTelefonicoAlertas, alertaGeneral );
//					break;
//				}
//				for( uint8_t x=0 ; x<100; x++)
//				{
//					buffRx[ x ] = 0x00;
//				}
//				HAL_Delay( 3000 );
//				break;
		}
}
