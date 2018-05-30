#include "humTemp.h"

void getHumTemp( uint8_t x ){

	int8_t value;
	char dhum1[]={"DHT11_1 Hum:000%"};
	char dhum2[]={"DHT11_2 Hum:000%"};
	char dtemp1[]={"DHT11_1 Temp:00C"};
	char dtemp2[]={"DHT11_2 Temp:00C"};
	
	if( x == 1)
	{
		DHT11( GPIO_PIN_6 );
		value = read(&humidity1,&humidity2 ,&temperature1 ,&temperature2);
		
		dhum1[12]=(humidity1/100)+0x30;						//Compute for the 1st digit
		dhum1[13]=((humidity1%100)/10)+0x30;			//Compute for the 2nd digit
		dhum1[14]=((humidity1%100)%10)+0x30;			//Compute for the 3rd digit

		dtemp1[13]=((temperature1%100)/10)+0x30;        //Compute for the 2nd digit
		dtemp1[14]=((temperature1%100)%10)+0x30;        //Compute for the 3rd digit
	}
	else if( x == 2 )
	{
		DHT11( GPIO_PIN_8 );
		value = read(&humidity1,&humidity2 ,&temperature1 ,&temperature2);
		
		dhum2[12]=(humidity1/100)+0x30;							//Compute for the 1st digit
		dhum2[13]=((humidity1%100)/10)+0x30;				//Compute for the 2nd digit
		dhum2[14]=((humidity1%100)%10)+0x30;				//Compute for the 3rd digit

		dtemp2[13]=((temperature1%100)/10)+0x30;		//Compute for the 2nd digit
		dtemp2[14]=((temperature1%100)%10)+0x30;		//Compute for the 3rd digit
	}
	
	if( value == retryLater )
	{
		LCD_clear();
		HAL_Delay(100);
		disp_enviar( "Error intente" , 0);
		disp_enviar( "de nuevo" , 1);
	}
	else if( value == readSuccesfully )
	{
		switch( x )
		{
			case 1:
				LCD_clear();
				HAL_Delay(150);
				disp_enviar( dhum1 , 0 );
				disp_enviar( dtemp1 , 1 );
				break;
			case 2:
				LCD_clear();
				HAL_Delay(150);
				disp_enviar( dhum2 , 0 );
				disp_enviar( dtemp2, 1 );
				break;
			default:
				break;
		}
	}
	else if( value == notReady)
	{
		LCD_clear();
		HAL_Delay(100);
		disp_enviar( "DHT not ready" , 0);
	}
	else if( value == checksumError)
	{
		LCD_clear();
		HAL_Delay(100);
		disp_enviar( "Validacion" , 0);
		disp_enviar( "Fallida" , 1);
	}
}
