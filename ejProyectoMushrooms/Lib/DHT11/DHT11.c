#include "DHT11.h"
#include "stm32f4xx_hal.h"

__weak uint32_t micros(void)
{
}

__weak uint32_t millis(void)
{
}

struct 
{
	uint16_t pin;
	uint32_t last_read_time;
}prv;


#define GPIO_PORT GPIOA

void pinMode( uint16_t pin , eMode mode )
{
	GPIO_InitTypeDef  gpio_init_structure;

	gpio_init_structure.Pin       = pin;
	gpio_init_structure.Mode      = (mode == INPUT) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_OD;
	gpio_init_structure.Pull      = GPIO_PULLUP;
	gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
	HAL_GPIO_Init( GPIO_PORT , &gpio_init_structure);
}

void digitalWrite(uint16_t pin, eEstado estado)
{
	HAL_GPIO_WritePin(GPIO_PORT,pin,(estado == LOW)?GPIO_PIN_RESET:GPIO_PIN_SET);
}

eEstado digitalRead( uint16_t pin )
{
	return (HAL_GPIO_ReadPin(GPIO_PORT,pin) == GPIO_PIN_RESET)?LOW:HIGH;
}

void DHT11( uint16_t pin_number ) 
{
	prv.pin = pin_number;
	prv.last_read_time = 0;
	pinMode(prv.pin,INPUT);
	digitalWrite(prv.pin, HIGH);
}

//wait for target status
//parameters
//  target : target status waiting for
//  time_out_us : time out in microsecond.
int32_t waitForT(eEstado target, uint32_t time_out_us) 
{
	uint32_t start = micros();
	uint32_t time_out = start + time_out_us;
	while( digitalRead( prv.pin ) != target )
	{
		if( time_out < micros()) return -1;
	}
	return micros() - start;
}

//wait for target status.
void waitFor( eEstado target ) 
{
	while( digitalRead( prv.pin ) != target );
}

extern void delay_us(uint32_t b);
	
//read one bye
uint8_t readByte( void ) 
{
	int8_t i;
	uint8_t ret = 0;
	for( i = 7 ; i >= 0 ; i-- )
	{
		waitFor(HIGH); //wait for 50us in LOW status
		delay_us(30); //wait for 30us
		if( digitalRead( prv.pin ) == HIGH ) //if HIGH status lasts for 30us, the bit is 1;
		{
			ret |= 1<<(i);
			waitFor( LOW ); //wait for rest time in HIGH status.
		}
	}
	return ret;
}

//parameters
//	temperature1 & temperature2: temperature to read.
//	humidity1 & humidity2 : humidity to read.
//return  0 : Read Successfully
//		  	1 : DHT11 not ready.
//      	2 : Checksum Error
//		  	3 : Read too shortly. Retry latter.

int8_t read(uint8_t *humidity1,uint8_t *humidity2 , uint8_t *temperature1 , uint8_t *temperature2) 
{
	uint8_t cksum;
	
	pinMode( prv.pin , OUTPUT );
	digitalWrite( prv.pin , LOW );
	HAL_Delay( 18 );
	digitalWrite( prv.pin , HIGH );
	pinMode( prv.pin,INPUT );

	if( waitForT( LOW , 40 ) < 0 )	return 1; //waiting for DHT11 ready
	if( waitForT( HIGH , 90 ) < 0 )	return 1; //waiting for first LOW signal(80us)
	if( waitForT( LOW , 90 ) < 0 )	return 1; //waiting for first HIGH signal(80us)

	*humidity1		= readByte();
	*humidity2		= readByte();
	*temperature1 = readByte();
	*temperature2 = readByte();
	
	cksum = readByte();
	
	if( *humidity1 + *humidity2 + *temperature1 + *temperature2 != cksum )
	{
		return 2;
	}
	
	//prv.last_read_time=millis();
	
	return 0;
}


