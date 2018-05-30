/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>

/*Control and display on the LCD 16x2*/
#include "STM_MY_LCD16X2.h"
#include "menu.h"
/*Control and display on the LCD 16x2*/

/*Sensor to measurement the different important variables for the cultivation*/
#include "DHT11.h"
#include "soilHumidity.h"
#include "light.h"
#include "humAmb.h"
#include "humComp.h"
#include "humTemp.h"
#include "tempAmb.h"
#include "tempComp.h"
#include "tempMCU.h"
/*Sensor to measurement the different important variables for the cultivation*/

/*Files that controlled and maintained the phase of the growing*/
#include "fase.h"
#include "ctrlFase.h"
/*Files that controlled and maintained the phase of the growing*/

/*Includes for control and working the module SIM800L*/
#include "AT.h"
#include "fibocom.h"
#include "responseSMS.h"
/*Includes for control and working the module SIM800L*/

#define CHECK 1

#define FAIL -1

#define timeout 2

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/*Variables to control the buttons and the LCD Display to initialization the system*/
typedef enum
{
	BREAK = -1,
	OFF,
	ON,
	RUN,
}eSystem;

typedef enum
{
	NOTHING,
	ARRIBA,
	ABAJO,
	OK,
}eButton;

typedef enum
{
	VOID = 0,
	MEDIUM,
	FULL,
}eLevel;

typedef enum
{
	datee,
	month,
	year,
	weekday,
}eDate;

typedef enum
{
	TEMPHUMAMB,
	TEMPSUELO,
	HUMSUELO,
	RTCCASE,
	TEMPSIM800,
	TEMPMCU,
	FAULT,
}eMED;

typedef enum
{
	MEDICION = 0,
	CONTROL,
	DATOS,
	SMSRX,
	SMSSEND,
	ALERTA,
	OUT,
	CALL,
}eProcess;

typedef enum
{
	JANUARY=1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
}eMonth;

eSystem system = OFF;
eButton msj = NOTHING;
eMED variable = TEMPHUMAMB;
uint8_t process = MEDICION;
uint8_t iTim = 0;
int8_t cntHMI = -1;
uint8_t cntHMI2 = 0;

/*Variables to control the buttons and the LCD Display to initialization the system*/

/*RS and E Control Pins to display in LCD 16x2*/
#define RS 8
#define E  5

#define D4 12
#define D5 15
#define D6 14
#define D7 13

const uint16_t LcdMaxRefreshTime = 500;
uint32_t LcdRefreshTime;
int8_t display = -1;
/*RS and E Control Pins to display in LCD 16x2*/

/*Buffer and variable to obtain the analogic values of Humidity, Illumination and the temperature
of the microcontroller*/
#define tam 6
uint8_t buffer[ tam ];
int16_t tempSensorMCU = 0;
float V25 = 760;
float Avg_Slope = 2500;
/*Buffer and variable to obtain the analogic values of Humidity, Illumination and the temperature
of the microcontroller*/

/*Variable to create a neccesary delay in microseconds to can connect with the DHT11 module sensor*/
volatile uint32_t a = 0;
volatile uint32_t us = 0;
volatile uint32_t u = 0;
/*Variable to create a neccesary delay in microseconds to can connect with the DHT11 module sensor*/

/*Array to storage the data of Date and time that will be necessary to control the period of cultivation*/
uint8_t Time[ 2 ];
uint8_t Date[ 4 ];
char timeLCD[] = {"   Time:00:00   "};
char dateLCD[] = {"  Date:00/00/00 "};
RTC_TimeTypeDef sTimeStructureGet;
RTC_DateTypeDef sDateStructureGet;
eFase date = faseGerminacion;			//Phase by default
uint16_t cntDate;
uint8_t setDate;
uint8_t setMonth;

typedef struct
{
	uint8_t yValue; 
	uint8_t yDigit1;
	uint8_t yDigit2;
	uint8_t mValue;
	uint8_t mDigit1;
	uint8_t mDigit2;
	uint8_t dValue;
	uint8_t dDigit1;
	uint8_t dDigit2;
	uint8_t hValue;
	uint8_t hDigit1;
	uint8_t hDigit2;
	uint8_t minValue;
	uint8_t minDigit1;
	uint8_t minDigit2;
}typedefDate;

typedefDate _year;

typedefDate _month;

typedefDate _date;

typedefDate _hour;

typedefDate _min;

/*Array to storage the data of Date and time that will be necessary to control the period of cultivation*/

/*Variable to control the level of tank and display through LCD 16x2 the correct level*/
eLevel sensorOptic = VOID;
uint8_t levelTank, g_countTank, cntAA, cntLight, cntHum, cntHumComp;
uint8_t alertaTank = 0;
/*Variable to control the level of tank and display through LCD 16x2 the correct level*/

/*Variable to measure a relative humidity of ambient and a temperature of ambient with DHT11*/
uint8_t DHT11_1 = 1;
uint8_t DHT11_2 = 2;
uint8_t humidity1, humidity2, temperature1, temperature11, temperature2;
uint8_t alertaHumLow, alertaHumHigh, alertaTempLow, alertaTempHigh, countTemp, countHum;
uint8_t lightHigh = 0;
uint8_t lightMedium = 0;
uint8_t g_countLight = 0;
/*Variable to measure a relative humidity of ambient and a temperature of ambient with DHT11*/

/*Variable to measure a humidity of the compost of growing*/
uint16_t hum1, hum2, humTotal;
/*Variable to measure a humidity of the compost of growing*/

/*Variable to measure the temperature at the compost of the growing*/
uint8_t crcMcu = 0;
uint8_t crcSensor = 0;
uint8_t g_countMcu = 0;
uint16_t tempComp1 = 0;
uint8_t alertaTempMcuHigh = 0;
uint8_t alertaTempMcuLow = 0;
/*Variable to measure the temperature at the compost of the growing*/

/*Configuration of module SIM800L to connect the service GPRS and GSM*/
uint8_t buffRx[ 2000 ];
uint8_t buffRxRead[ 500 ];
int8_t state = -100;
uint8_t receiveMsj = 9;
int8_t estado;
bool msg = false;
char numeroTelefonicoAlertas[] = {"+584243458044"};
char numeroTelefonico[] = {"+584000000000"};
char temperaturaAmbiental[] = {"Temperatura Ambiental: 00°C. Cultivos Mushrooms."};
char humedadAmbiental[] = {"Humedad Ambiental: 000%. Cultivos Mushrooms."};
char temperaturaSustrato[] = {"Temperatura del Sustrato: 00°C. Cultivos Mushrooms."};
char humedadSustrato[] = {"Humedad del Sustrato: 000%. Cultivos Mushrooms."};
char iluminacion[] = {"Iluminacion del entorno: Optima. Cultivos Mushrooms."};
char nivelTanque[] = {"Nivel del tanque: Vacio. Cultivos Mushrooms."};
char temperaturaSim800[] = {"Temperatura del modulo: 00°C. Cultivos Mushrooms."};
char temperaturaMCU[] = {"Temperatura del MCU: 00°C. Cultivos Mushrooms."};
char msjAllVariables[] = {"Temperatura Ambiental: 00°C. Humedad Ambiental: 000%. Temperatura del sustrato: 000°C. Humedad del sustrato: 000%. Iluminacion: Optima. Nivel del tanque: Vacio. Temperatura del sim800: 00°C. Temperatura del MCU: 00°C. Cultivos Mushrooms"};
char errorMsj[] = {"Error de codigo, Intente de nuevo"};
char alertaTempMsj[] = {"Alerta temperatura ambiental fuera de sus rangos"};
char alertaHum[] = {"Alerta humedad ambiental fuera de sus rangos"};
char alertaIlumAlta[] = {"Alerta iluminacion excesiva"};
char alertaTempMCU[] = {"Alerta temperatura del sistema en peligro apagado de proteccion del sistema!!!"};
char alertaGeneral[] = {"Alerta en el sistema, variable fuera de rango. Peligro¡¡¡"};
uint8_t call = 0;
int8_t stateData = -2;
int8_t SimAccess = FAIL;
char updtVariables[] = { "api_key=4GUTFU81Z71XC9XR&field1=000&field2=000&field3=000&field4=000&field5=000&field6=000&field7=0" };
uint8_t g_cntData = 0;
uint8_t g_countAlarm = 0;
uint8_t g_errorCount = 0;
bool b_firstRound = false;
uint8_t retryHTTP = 0;

/*Configuration of module SIM800L to connect the service GPRS and GSM*/

/*Arrays to storage the data of the variables*/
typedef struct
{
	uint8_t Amb1h[ 11 ];
	
	uint8_t Amb2h[ 11 ];
	
	uint8_t Amb1t[ 11 ];
	
	uint8_t Amb2t[ 11 ];
	
	uint8_t Soil[ 11 ];
	
	uint8_t Soil1h[ 11 ];
	
	uint8_t Soil2h[ 11 ];
	
	uint8_t MCU[ 11 ];
	
	uint8_t SIM800[ 11 ];
	
}arrayVariable;

arrayVariable temp;

arrayVariable hum;

typedef struct
{

  uint8_t TempHumAmb;         

  uint8_t TempSoil;         

  uint8_t HumSoil;          

  uint8_t TempSim800;

  uint8_t TempMCU;
	
	uint8_t Dht;
	
	uint8_t digit1;
	
	uint8_t digit2;
	
	uint8_t value;

}indexTypedef;

indexTypedef iVariable;

indexTypedef cond;

indexTypedef erase;

/*Arrays to storage the data of the variables*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/*Prototypes of function*/
extern void controlAmbiental( void );
extern void disp_enviar( char* text, int linea );
uint16_t  ReportTemperature( void );
void  OneWire_Init( void );
void timeRTC ( void );
void dateRTC ( void );

extern int8_t Inicializacion( void );
extern int8_t initGSM( void );
extern void Send( char *_number, char *_message );
extern void SMSquery( uint8_t receiveMsj );

extern int8_t initGPRS( void );
extern int8_t GETHTTP( char *CID, char *URL );
extern int8_t POSTHTTP( char *CID, char *URL, char *sizeData, char *latency, char *Data );
/*Prototypes of function*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART6_UART_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
	
	HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_SET );
	
  /*Initialization of LCD Display 16x2*/
  LCD_begin4BIT( GPIOC, RS, E, GPIOB, D4, D5, D6, D7 );
	LCD_clear();
	LCD_noCursor();
	HAL_Delay( 10 );
	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_11, GPIO_PIN_SET );
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_12, GPIO_PIN_SET );
	uint8_t retryConfig1 = 0;
  /*Initialization of LCD Display 16x2*/
	
	fibocom_bsp_init();
	
	Cmd_ATQ( "0" );
	HAL_Delay( 1000 );
		
	for( uint8_t x=0 ; x<100; x++)
	{
		buffRx[ x ] = 0x00;
	}
	
	while( ( SimAccess == FAIL ) && ( retryConfig1 < 10 ) )
	{
		Cmd_ATCSMINS();
		HAL_Delay( 2000 );
		
		if( ( ( buffRx[22] == '0' ) && ( buffRx[24] == '1' ) && ( buffRx[29] == 'O' ) && ( buffRx[30] == 'K') ) || ( ( buffRx[12] == '0' ) && ( buffRx[14] == '1' ) ) )
		{
			SimAccess = CHECK;
			for( uint8_t x=0 ; x<100; x++)
			{
				buffRx[ x ] = 0x00;
			}
			retryConfig1 = 0;
		}
		else
		{
			SimAccess = FAIL;
			for( uint8_t x=0 ; x<100; x++)
			{
				buffRx[ x ] = 0x00;
			}
			disp_enviar( "  Inserte SIM..." , 0 );
			HAL_Delay( 50 );
			disp_enviar( "                " , 1 );
			HAL_Delay( 3000 );
			retryConfig1++;
		}
	}
	
	if( retryConfig1 == 0 )
	{
		while( state != 0 )
		{
			disp_enviar( " Inicializando  ", 0 );
			disp_enviar( "    Red Gsm...  ", 1 );
			HAL_Delay( 150 );
			state = Inicializacion();
			switch( state )
			{
				case 0:
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					retryConfig1 = 0;
					break;
				case -1:
					retryConfig1++;
					if( retryConfig1 == 60 )
					{
						LCD_clear();
						HAL_Delay( 150 );
						LCD_clear();
						HAL_Delay( 150 );
						disp_enviar( " Reiniciando en ", 0 );
						HAL_Delay( 150 );
						disp_enviar( "       5        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       4        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       3        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       2        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       1        ", 1 );
						HAL_Delay( 1000 );
					
						HAL_NVIC_SystemReset();
					}
					break;
				default:
					break;
			}
		}
	
		while( state == 0 )
		{
			LCD_blink();
			disp_enviar( " Configurando...", 0 );
			disp_enviar( "               ", 1 );
			HAL_Delay( 150 );
			state = initGSM();
			switch( state )
			{
				case -1:
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					retryConfig1 = 0;
					break;
				case 0:
					retryConfig1++;
					if( retryConfig1 == 60 )
					{
						LCD_clear();
						HAL_Delay( 150 );
						disp_enviar( " Reiniciando en ", 0 );
						HAL_Delay( 150 );
						disp_enviar( "       5        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       4        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       3        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       2        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       1        ", 1 );
						HAL_Delay( 1000 );
					
						HAL_NVIC_SystemReset();
					}
					break;
				default:
					break;
			}
			LCD_clear();
			HAL_Delay( 150 );
		}
	
		while( state != 0 )
		{
			disp_enviar( "  Configurando  ", 0 );
			disp_enviar( "      GPRS...  ", 1 );
			HAL_Delay( 150 );
			state = initGPRS();
			switch( state )
			{
				case 0:
					for( uint8_t x=0 ; x<100; x++)
					{
						buffRx[ x ] = 0x00;
					}
					retryConfig1 = 0;
					break;
				case -1:
					retryConfig1++;
					if( retryConfig1 == 60 )
					{
						LCD_clear();
						HAL_Delay( 150 );
						disp_enviar( " Reiniciando en ", 0 );
						HAL_Delay( 150 );
						disp_enviar( "       5        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       4        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       3        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       2        ", 1 );
						HAL_Delay( 1000 );
						disp_enviar( "       1        ", 1 );
						HAL_Delay( 1000 );
					
						HAL_NVIC_SystemReset();
					}
					break;
				default:
					break;
			}
			LCD_clear();
			HAL_Delay( 150 );
		}
	
		while( state == 0 )
		{
			disp_enviar( "   Obteniendo  ", 0 );
			disp_enviar( "     Datos...  ", 1 );
			HAL_Delay( 150 );
			CIPGSMLOC( "2", "1" );
			HAL_Delay( 5000 );
			if( ( buffRx[ 2 ] == 'E' ) && ( buffRx[ 3 ] == 'R' ) && ( buffRx[ 4 ] == 'R' ) && ( buffRx[ 5 ] == 'O' ) && ( buffRx[ 6 ] == 'R' ) )
			{
				state = 0;
			}
			else if( buffRx[ 14 ] == '0' )
			{
				for( uint8_t i = 0; i < 10; i++ )
				{
					if( buffRx[ 18 ] == ( i + 0x30 ) ) _year.yDigit1 = i*10;
					if( buffRx[ 19 ] == ( i + 0x30 ) ) _year.yDigit2 = i;
					_year.yValue = _year.yDigit1 + _year.yDigit2;
			
					if( buffRx[ 21 ] == ( i + 0x30 ) ) _month.mDigit1 = i*10;
					if( buffRx[ 22 ] == ( i + 0x30 ) ) _month.mDigit2 = i;
					_month.mValue = _month.mDigit1 + _month.mDigit2;
			
					if( buffRx[ 24 ] == ( i + 0x30 ) ) _date.dDigit1 = i*10;
					if( buffRx[ 25 ] == ( i + 0x30 ) ) _date.dDigit2 = i;
					_date.dValue = _date.dDigit1 + _date.dDigit2;
			
					if( buffRx[ 27 ] == ( i + 0x30 ) ) _hour.hDigit1 = i*10;
					if( buffRx[ 28 ] == ( i + 0x30 ) ) _hour.hDigit2 = i;
					_hour.hValue = _hour.hDigit1 + _hour.hDigit2;
				
					if( ( _hour.hValue >= 0x00 ) && (_hour.hValue <= 0x03 ) ) 
					{
						_date.dValue = _date.dValue - 1;
						switch( _hour.hValue )
						{
							case 0:
								_hour.hValue = 0x08;
								break;
							case 1:
								_hour.hValue = 0x09;
								break;
							case 2:
								_hour.hValue = 0x0A;
								break;
							case 3:
								_hour.hValue = 0x0B;
								break;
							default:
								break;
						}
					}
					else
					{
						_hour.hValue = _hour.hValue - 4;
					}
			
					if( buffRx[ 30 ] == ( i + 0x30 ) ) _min.minDigit1 = i*10;
					if( buffRx[ 31 ] == ( i + 0x30 ) ) _min.minDigit2 = i;
					_min.minValue = _min.minDigit1 + _min.minDigit2;
				}
				state = -1;
			}
			else
			{
				state = 0;
				for( uint8_t x=0 ; x<100; x++)
				{
					buffRx[ x ] = 0x00;
				}
			}
		}
	
		LCD_noBlink();
	
		switch( _month.mValue )
		{
			case JANUARY:
				_month.mValue = 0x01;
				break;
			case FEBRUARY:
				_month.mValue = 0x02;
				break;
			case MARCH:
				_month.mValue = 0x03;
				break;
			case APRIL:
				_month.mValue = 0x04;
				break;
			case MAY:
				_month.mValue = 0x05;
				break;
			case JUNE:
				_month.mValue = 0x06;
				break;
			case JULY:
				_month.mValue = 0x07;
				break;
			case AUGUST:
				_month.mValue = 0x08;
				break;
			case SEPTEMBER:
				_month.mValue = 0x09;
				break;
			case OCTOBER:
				_month.mValue = 0x10;
				break;
			case NOVEMBER:
				_month.mValue = 0x11;
				break;
			case DECEMBER:
				_month.mValue = 0x12;
				break;
			default:
				break;
		}
	}
	else
	{
		LCD_clear();
		LCD_blink();
		HAL_Delay( 50 );
		disp_enviar( "    Sistema     " , 0 );
		HAL_Delay( 150 );
		disp_enviar( "  Sin Conexion  " , 1 );
		HAL_Delay( 3000 );
	}
	
	MX_RTC_Init();
	
	LCD_noBlink();
	
	/*Enable the RTC function and the Backup Domain Access*/
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_RTC_ENABLE();
	/*Enable the RTC function and the Backup Domain Access*/

	/*Set the date and the time of the RTC to control the days of the growing*/
	dateRTC();
	HAL_Delay( 100 );
	timeRTC();
	setDate = Date[ datee ];
	setMonth = Date[ month ];
	/*Set the date and the time of the RTC to control the days of the growing*/
	
	/*Turn on the ADC through the DMA and the Timer 5*/
  HAL_ADC_Start_DMA( &hadc1, (uint32_t *)buffer, tam );
  HAL_TIM_Base_Start( &htim5 );
  /*Turn on the ADC through the DMA and the Timer 5*/
	
	for( uint8_t x=0 ; x<100; x++)
	{
		buffRx[ x ] = 0x00;
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		 do{
      switch ( system )
			{
				case OFF:
          HAL_GPIO_WritePin( GPIOC, GPIO_PIN_11, GPIO_PIN_SET );
          HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );
					HAL_GPIO_WritePin( GPIOA, GPIO_PIN_12, GPIO_PIN_RESET );
					dateRTC();
					timeRTC();
          break;
				case ON:
					HAL_TIM_Base_Start_IT( &htim2 );
          HAL_GPIO_WritePin( GPIOC, GPIO_PIN_11, GPIO_PIN_RESET );
          HAL_GPIO_WritePin( GPIOC, GPIO_PIN_10, GPIO_PIN_SET );
					HAL_GPIO_WritePin( GPIOA, GPIO_PIN_12, GPIO_PIN_SET );
					mnu_mostrar_menu( MenuPrincipal2, 0, LINEA_SUPERIOR );
					system = BREAK;
					break;
				case RUN:
					LCD_noCursor();
					HAL_TIM_Base_Start_IT( &htim2 );
					if( cntDate == 2770 )
					{
						date = cultivation( date );
						cntDate = 0;
					}
					if( cntHMI == 0 )
					{
						dateRTC();
						HAL_Delay( 100 );
						timeRTC();
						date = cultivation( date );
					}
					switch( process )
					{
						case MEDICION:
							switch( variable )
							{
								case TEMPHUMAMB:
									DHT11( GPIO_PIN_6 );
									cond.Dht = read( &humidity1, &humidity2, &temperature1, &temperature2 );
									if( erase.TempHumAmb != false )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											hum.Amb1h[ x ] = 0;
											temp.Amb1t[ x ] = 0;
										}
									}
									if( cond.Dht == 0 )
									{
										hum.Amb1h[ iVariable.TempHumAmb ] = humidity1;
										temp.Amb1t[ iVariable.TempHumAmb ] = temperature1;
									}
									HAL_Delay( 1000 );
									DHT11( GPIO_PIN_8 );
									cond.Dht = ( uint8_t ) read( &humidity1, &humidity2, &temperature1, &temperature2 );
									if( erase.TempHumAmb != false )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											hum.Amb2h[ x ] = 0;
											temp.Amb2t[ x ] = 0;
										}
										erase.TempHumAmb = false;
									}
									if( cond.Dht == 0 )
									{
										hum.Amb2h[ iVariable.TempHumAmb ] = humidity1;
										temp.Amb2t[ iVariable.TempHumAmb ] = temperature1;
									}
									temperaturaAmbiental[ 23 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									temperaturaAmbiental[ 24 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									msjAllVariables[ 23 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									msjAllVariables[ 24 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									humedadAmbiental[ 19 ] = ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) / 100 ) + 0x30;
									humedadAmbiental[ 20 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									humedadAmbiental[ 21 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									msjAllVariables[ 48 ] = ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) / 100 ) + 0x30;
									msjAllVariables[ 49 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									msjAllVariables[ 50 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									iVariable.TempHumAmb++;
									if( iVariable.TempHumAmb == 10 )
									{
										iVariable.TempHumAmb = 0;
										hum.Amb1h[ 10 ] = ( ( hum.Amb1h[ 0 ] + hum.Amb1h[ 1 ] + hum.Amb1h[ 2 ] + hum.Amb1h[ 3 ] + hum.Amb1h[ 4 ] + hum.Amb1h[ 5 ] + hum.Amb1h[ 6 ] + hum.Amb1h[ 7 ] + hum.Amb1h[ 8 ] + hum.Amb1h[ 9 ] ) / 10 );
										temp.Amb1t[ 10 ] = ( ( temp.Amb1t[ 0 ] + temp.Amb1t[ 1 ] + temp.Amb1t[ 2 ] + temp.Amb1t[ 3 ] + temp.Amb1t[ 4 ] + temp.Amb1t[ 5 ] + temp.Amb1t[ 6 ] + temp.Amb1t[ 7 ] + temp.Amb1t[ 8 ] + temp.Amb1t[ 9 ] ) / 10 );
										temperaturaAmbiental[ 23 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
										temperaturaAmbiental[ 24 ] = ( ( ( ( ( temp.Amb1t[ iVariable.TempHumAmb ] ) + ( temp.Amb2t[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
										humedadAmbiental[ 19 ] = ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) / 100 ) + 0x30;
										humedadAmbiental[ 20 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
										humedadAmbiental[ 21 ] = ( ( ( ( ( hum.Amb1h[ iVariable.TempHumAmb ] ) + ( hum.Amb2h[ iVariable.TempHumAmb ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
										erase.TempHumAmb = true;
									}
									variable = TEMPSUELO;
								case TEMPSUELO:
									if( ( erase.TempSoil != false ) )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											temp.Soil[ x ] = 0;
										}
										erase.TempSoil = false;
									}
									temp.Soil[ iVariable.TempSoil ] = ReportTemperature();
									if( 200 != temp.Soil[ iVariable.TempSoil ] )
									{
										temp.Soil[ iVariable.TempSoil ] = ReportTemperature();
										temperaturaSustrato[ 26 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) / 10 ) + 0x30;
										temperaturaSustrato[ 27 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) % 10 ) + 0x30;
										msjAllVariables[ 80 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) / 10 ) + 0x30;
										msjAllVariables[ 81 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) % 10 ) + 0x30;
										iVariable.TempSoil++;
									}
									else
									{
										temp.Soil[ iVariable.TempSoil ] = temp.Soil[ iVariable.TempSoil - 1 ];	
										temperaturaSustrato[ 26 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) / 10 ) + 0x30;
										temperaturaSustrato[ 27 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) % 10 ) + 0x30;
										msjAllVariables[ 81 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) / 10 ) + 0x30;
										msjAllVariables[ 82 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) % 10 ) + 0x30;							
										iVariable.TempSoil++;
									}
									if( iVariable.TempSoil == 10 )
									{
										temp.Soil[ 10 ] = ( ( temp.Soil[ 0 ] + temp.Soil[ 1 ] + temp.Soil[ 2 ] + temp.Soil[ 3 ] + temp.Soil[ 4 ] + temp.Soil[ 5 ] + temp.Soil[ 6 ] + temp.Soil[ 7 ] + temp.Soil[ 8 ] + temp.Soil[ 9 ] ) / 10 );
										temperaturaSustrato[ 26 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) / 10 ) + 0x30;
										temperaturaSustrato[ 27 ] = ( ( temp.Soil[ iVariable.TempSoil ] % 100 ) % 10 ) + 0x30;
										iVariable.TempSoil = 0;
										erase.TempSoil = true;
									}
									variable = HUMSUELO;
								case HUMSUELO:
									if( erase.HumSoil != false )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											hum.Soil1h[ x ] = 0;
											hum.Soil2h[ x ] = 0;
										}
									erase.HumSoil = false;
									}
									getSoilHumidity();
									hum.Soil1h[ iVariable.HumSoil ] = hum1;
									hum.Soil2h[ iVariable.HumSoil ] = hum2;
									humedadSustrato[ 22 ] = ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) / 100 ) + 0x30;
									humedadSustrato[ 23 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									humedadSustrato[ 24 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									msjAllVariables[ 108 ] = ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) / 100 ) + 0x30;
									msjAllVariables[ 109 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
									msjAllVariables[ 110 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
									iVariable.HumSoil++;
									if( iVariable.HumSoil == 10 )
									{
										hum.Soil1h[ 10 ] = ( ( hum.Soil1h[ 0 ] + hum.Soil1h[ 1 ] + hum.Soil1h[ 2 ] + hum.Soil1h[ 3 ] + hum.Soil1h[ 4 ] + hum.Soil1h[ 5 ] + hum.Soil1h[ 6 ] + hum.Soil1h[ 7 ] + hum.Soil1h[ 8 ] + hum.Soil1h[ 9 ] ) / 10 );
										hum.Soil2h[ 10 ] = ( ( hum.Soil2h[ 0 ] + hum.Soil2h[ 1 ] + hum.Soil2h[ 2 ] + hum.Soil2h[ 3 ] + hum.Soil2h[ 4 ] + hum.Soil2h[ 5 ] + hum.Soil2h[ 6 ] + hum.Soil2h[ 7 ] + hum.Soil2h[ 8 ] + hum.Soil2h[ 9 ] ) / 10 );
										humedadSustrato[ 22 ] = ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) / 100 ) + 0x30;
										humedadSustrato[ 23 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
										humedadSustrato[ 24 ] = ( ( ( ( ( hum.Soil1h[ iVariable.HumSoil ] ) + ( hum.Soil2h[ iVariable.HumSoil ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
										iVariable.HumSoil = 0;
										erase.HumSoil = true;
									}
									variable = TEMPSIM800;
								case TEMPSIM800:
									if( ( erase.TempSim800 != false ) )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											temp.SIM800[ x ] = 0;
										}
										erase.TempSim800 = false;
									}
									CMTE( "?" );
									HAL_Delay( 1000 );
									for( uint8_t i = 0; i<10; i++ )
									{
										if( ( i + 0x30 ) == buffRx[ 11 ] ) iVariable.digit1 = i*10;
										if( ( i + 0x30 ) == buffRx[ 12 ] ) iVariable.digit2 = i;
										iVariable.value = iVariable.digit1 + iVariable.digit2;
									}
									temp.SIM800[ iVariable.TempSim800 ] = iVariable.value;
									temperaturaSim800[ 24 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) / 10 ) + 0x30;
									temperaturaSim800[ 25 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) % 10 ) + 0x30;
									msjAllVariables[ 185 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) / 10 ) + 0x30;
									msjAllVariables[ 186 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) % 10 ) + 0x30;
									iVariable.TempSim800++;
									for( uint16_t x=0 ; x<50; x++)
									{
										buffRx[ x ] = 0x00;
									}
									if( iVariable.TempSim800 == 10 )
									{
										temp.SIM800[ 10 ] = ( ( temp.SIM800[ 0 ] + temp.SIM800[ 1 ] + temp.SIM800[ 2 ] + temp.SIM800[ 3 ] + temp.SIM800[ 4 ] + temp.SIM800[ 5 ] + temp.SIM800[ 6 ] + temp.SIM800[ 7 ] + temp.SIM800[ 8 ] + temp.SIM800[ 9 ] ) / 10 );
										temperaturaSim800[ 24 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) / 10 ) + 0x30;
										temperaturaSim800[ 25 ] = ( ( temp.SIM800[ iVariable.TempSim800 ] % 100 ) % 10 ) + 0x30;
										iVariable.TempSim800 = 0;
										erase.TempSim800 = true;
									}
								case TEMPMCU:
									if( ( erase.TempMCU != false ) )
									{
										for( uint8_t x = 0; x<10; x++)
										{
											temp.MCU[ x ] = 0;
										}
										erase.TempMCU = false;
									}
									tempSensor();
									temp.MCU[ iVariable.TempMCU ] = tempSensorMCU;
									temperaturaMCU[ 21 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) / 10 ) + 0x30;
									temperaturaMCU[ 22 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) % 10 ) + 0x30;
									msjAllVariables[ 212 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) / 10 ) + 0x30;
									msjAllVariables[ 213 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) % 10 ) + 0x30;
									iVariable.TempMCU++;
									if( iVariable.TempMCU == 10 )
									{
										temp.MCU[ 10 ] = ( ( temp.MCU[ 0 ] + temp.MCU[ 1 ] + temp.MCU[ 2 ] + temp.MCU[ 3 ] + temp.MCU[ 4 ] + temp.MCU[ 5 ] + temp.MCU[ 6 ] + temp.MCU[ 7 ] + temp.MCU[ 8 ] + temp.MCU[ 9 ] ) / 10 );
										temperaturaMCU[ 21 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) / 10 ) + 0x30;
										temperaturaMCU[ 22 ] = ( ( temp.MCU[ iVariable.TempMCU ] % 100 ) % 10 ) + 0x30;
										iVariable.TempMCU = 0;
										erase.TempMCU = true;
									}
									if( ( iTim == 5 ) || ( iTim == 10 ) || ( iTim == 2 ) )
									{
										process = CONTROL; 
									}
									else if( msg != true )
									{
										process = OUT;
									}
									else
									{
										process = SMSRX;
									}
									variable = FAULT;
									break;
								default:
									break;
							}
							break;
						case CONTROL:
							controlAmbiental();
							if( msg == true )
							{
								process = SMSRX;
							}
							else if( ( iTim == 5 ) || ( iTim == 2 ) )
							{
								process = OUT;
							}
							else if( iTim == 10 )
							{
								process = DATOS;
							}
							break;
						case DATOS:
							HAL_TIM_Base_Stop_IT( &htim2 );
							while( state != 0 )
							{
								state = initGPRS();
								switch( state )
								{
									case 0:
										for( uint8_t x=0 ; x<100; x++)
										{
											buffRx[ x ] = 0x00;
										}
										break;
									case -1:
										break;
									default:
										break;
								}
								HAL_Delay( 150 );
							}
							updtVariables[ 32 ] = ( ( ( ( hum.Amb1h[ 10 ] ) + ( hum.Amb2h[ 10 ] ) ) / 2 ) / 100 ) + 0x30;
							updtVariables[ 33 ] = ( ( ( ( ( hum.Amb1h[ 10 ] ) + ( hum.Amb2h[ 10 ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
							updtVariables[ 34 ] = ( ( ( ( ( hum.Amb1h[ 10 ] ) + ( hum.Amb2h[ 10 ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
							updtVariables[ 43 ] = ( ( ( ( hum.Soil1h[ 10 ] ) + ( hum.Soil2h[ 10 ] ) ) / 2 ) / 100 ) + 0x30;
							updtVariables[ 44 ] = ( ( ( ( ( hum.Soil1h[ 10 ] ) + ( hum.Soil2h[ 10 ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
							updtVariables[ 45 ] = ( ( ( ( ( hum.Soil1h[ 10 ] ) + ( hum.Soil2h[ 10 ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
							updtVariables[ 54 ] = ( ( ( ( temp.Amb1t[ 10 ] ) + ( temp.Amb2t[ 10 ] ) ) / 2 ) / 100 ) + 0x30;
							updtVariables[ 55 ] = ( ( ( ( ( temp.Amb1t[ 10 ] ) + ( temp.Amb2t[ 10 ] ) ) / 2 ) % 100 ) / 10 ) + 0x30;
							updtVariables[ 56 ] = ( ( ( ( ( temp.Amb1t[ 10 ] ) + ( temp.Amb2t[ 10 ] ) ) / 2 ) % 100 ) % 10 ) + 0x30;
							updtVariables[ 65 ] = ( ( temp.Soil[ 10 ] ) / 100 ) + 0x30;
							updtVariables[ 66 ] = ( ( temp.Soil[ 10 ] % 100 ) / 10 ) + 0x30;
							updtVariables[ 67 ] = ( ( temp.Soil[ 10 ] % 100 ) % 10 ) + 0x30;
							updtVariables[ 76 ] = ( ( temp.SIM800[ 10 ] ) / 100 ) + 0x30;
							updtVariables[ 77 ] = ( ( temp.SIM800[ 10 ] % 100 ) / 10 ) + 0x30;
							updtVariables[ 78 ] = ( ( temp.SIM800[ 10 ] % 100 ) % 10 ) + 0x30;
							updtVariables[ 87 ] = ( ( temp.MCU[ 10 ] ) / 100 ) + 0x30;
							updtVariables[ 88 ] = ( ( temp.MCU[ 10 ] % 100 ) / 10 ) + 0x30;
							updtVariables[ 89 ] = ( ( temp.MCU[ 10 ] % 100 ) % 10 ) + 0x30;
							updtVariables[ 98 ] = ( ( date ) / 100 ) + 0x30;
							while( ( stateData < 0 ) && ( retryHTTP < 5 ) )
							{
								stateData = POSTHTTP( "1", "api.thingspeak.com/update", "99", "10000", updtVariables );
								g_cntData++;
								retryHTTP++;
							}
							if( ( retryHTTP >= 5 ) && ( stateData == -1 ) )
							{
								disp_enviar( "FallaTransmision", 0 );
								HAL_Delay( 150 );
								disp_enviar( "     Datos      ", 1 );
							}
							retryHTTP = 0;
							HAL_Delay( 5000 );
							b_firstRound = true;
							stateData = -1;
							if( true == msg )
							{
								process = SMSRX;
							}
							else if( ( countHum >= timeout ) || ( countTemp >= timeout ) || ( g_countLight >= timeout )
										|| ( g_countTank >= timeout ) || (g_countMcu >= timeout ) )
							{
								process = ALERTA;
							}
							else
							{
								HAL_TIM_Base_Start_IT( &htim2 );
								process = OUT;
							}
							break;
						case SMSRX:
							HAL_TIM_Base_Stop_IT( &htim2 );
							SMSquery( smsRX );
							process = OUT;
							HAL_TIM_Base_Start_IT( &htim2 );
							break;
						case SMSSEND:
							HAL_TIM_Base_Stop_IT( &htim2 );
							SMSquery( smsTX );
							process = MEDICION;
							HAL_TIM_Base_Start_IT( &htim2 );
							break;
						case ALERTA:
							HAL_TIM_Base_Stop_IT( &htim2 );
							SMSquery( smsTX );
							process = MEDICION;
							HAL_TIM_Base_Start_IT( &htim2 );
							break;
						case CALL:
							HAL_TIM_Base_Stop_IT( &htim2 );
							HAL_Delay( 2000 );
							Cmd_ATA();
							HAL_Delay( 2000 );
							Cmd_ATH();
							call = 0;
							process = MEDICION;
							for( uint8_t x=0 ; x<100; x++)
							{
								buffRx[ x ] = 0x00;
							}
							HAL_TIM_Base_Start_IT( &htim2 );
							break;
						default:
							break;
						}
					break;
				case BREAK:
					if( ( ( iTim == 2 ) || ( iTim == 4 ) || ( iTim == 6 ) || ( iTim == 8 ) ) && ( ( display == -1 )  || ( display == 4 ) ) && ( cntHMI != -2 ) )
					{
						dateRTC();
						HAL_Delay( 100 );
						timeRTC();
						date = fase( faseGerminacion );
						cntHMI = 0;
						system = RUN;
					}
					break;
				default:
					break;
			}
      if( msj == ARRIBA )
      {
				if( ( system == RUN ) && ( cntHMI == 0 ) )
				{
					mnu_mostrar_menu( MenuPrincipal, 0, LINEA_SUPERIOR );
					cntHMI = -1;
					msj = NOTHING;
				}
				else
				{
					cntHMI2 = 0;
					mnu_ejecutar_funcion( TECLA_ARRIBA );
					msj = NOTHING;
					display = 0;
					iTim = 0;
				}
      }
      else if( msj == ABAJO )
      {
				if( ( system == RUN ) && ( cntHMI == 0 ) )
				{
					mnu_mostrar_menu( MenuPrincipal, 0, LINEA_SUPERIOR );
					cntHMI = -1;
					msj = NOTHING;
				}
				else
				{
					cntHMI2 = 0;
					mnu_ejecutar_funcion( TECLA_ABAJO );
					msj = NOTHING;
					display = 0;
					iTim = 0;
				}
      }
      else if( msj == OK )
      {
				if( ( system == RUN ) && ( cntHMI == 0 ) )
				{
					mnu_mostrar_menu( MenuPrincipal, 0, LINEA_SUPERIOR );
					cntHMI = -1;
					msj = NOTHING;
				}
				else
				{
					cntHMI2 = 0;
					mnu_ejecutar_funcion( TECLA_OK );
					msj = NOTHING;
					display = 0;
					iTim = 0;
				}
      }
      
      /*Ejecutar cada 500ms*/
     if(( HAL_GetTick() - LcdRefreshTime ) > LcdMaxRefreshTime )
     {
       mnu_ejecutar_funcion_periodica();
       LcdRefreshTime = HAL_GetTick();
     }
      /*Ejecutar cada 500ms*/
    }while( system != OFF );
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		/*SHUTTING DOWN OF THE CONTROLS*/
		HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET );
		HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET );
		HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7, GPIO_PIN_SET );
		/*SHUTTING DOWN OF THE CONTROLS*/
	}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  sTime.Hours = _hour.hValue;
  sTime.Minutes = _min.minValue;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
  sDate.Month = _month.mValue;
  sDate.Date = _date.dValue;
  sDate.Year = _year.yValue;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 65535;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 40000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM5 init function */
static void MX_TIM5_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xffffffff;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, resCalefAmb_Pin|valAdmi_Pin|oneWireDS18B20_Pin|A_LCD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, E_LCD_Pin|RS_LCD_Pin|ledGreenOn_Pin|ledRedOff_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Humif_Pin|D4_LCD_Pin|D7_LCD_Pin|D6_LCD_Pin 
                          |D5_LCD_Pin|resCalefComp_Pin|valExtr_Pin|door_Pin 
                          |A_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : buttonOk_Pin buttonUp_Pin buttonDown_Pin Si_Pin */
  GPIO_InitStruct.Pin = buttonOk_Pin|buttonUp_Pin|buttonDown_Pin|Si_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : resCalefAmb_Pin valAdmi_Pin oneWireDS18B20_Pin A_LCD_Pin */
  GPIO_InitStruct.Pin = resCalefAmb_Pin|valAdmi_Pin|oneWireDS18B20_Pin|A_LCD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DHT11_1_Pin DHT11_2_Pin */
  GPIO_InitStruct.Pin = DHT11_1_Pin|DHT11_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : E_LCD_Pin RS_LCD_Pin ledGreenOn_Pin ledRedOff_Pin */
  GPIO_InitStruct.Pin = E_LCD_Pin|RS_LCD_Pin|ledGreenOn_Pin|ledRedOff_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Sf_Pin buttonOnOff_Pin */
  GPIO_InitStruct.Pin = Sf_Pin|buttonOnOff_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Humif_Pin D4_LCD_Pin D7_LCD_Pin D6_LCD_Pin 
                           D5_LCD_Pin resCalefComp_Pin valExtr_Pin door_Pin 
                           A_A_Pin */
  GPIO_InitStruct.Pin = Humif_Pin|D4_LCD_Pin|D7_LCD_Pin|D6_LCD_Pin 
                          |D5_LCD_Pin|resCalefComp_Pin|valExtr_Pin|door_Pin 
                          |A_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
uint32_t micros(void){
  return htim5.Instance->CNT;
}

void delay_us(uint32_t us){
  uint32_t tInit = micros();
  while((micros() - tInit) < us);
}

/*
 *  OneWire_Init      hardware-specific configuration of 1-wire I/O
 */

void  OneWire_Init(void)
{
  pinMode( GPIO_PIN_9, OUTPUT );
}

static void  SendInitialization(void)
{
	
  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_SET );
  pinMode( GPIO_PIN_9, OUTPUT );  
  delay_us( 500 );

  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_RESET );
  delay_us( 500 );

  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_SET );
  pinMode( GPIO_PIN_9, INPUT );
  delay_us( 50 );
}


static void  SendByte( uint8_t  val )
{
    uint8_t n;

    for ( n = 0; n < 8; n++ )
    {
        pinMode( GPIO_PIN_9, OUTPUT );
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_RESET );
        delay_us( 5 );
        if (val & 1)  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_SET );
        delay_us( 95 );
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_SET );
        delay_us( 5 );
        val = val >> 1;
    }
}

static  uint8_t  ReadByte( void )
{
    uint8_t n;
    uint8_t val;

    val = 0;
    for ( n = 0; n<8; n++ )
    {
        val = val >> 1;
        pinMode( GPIO_PIN_9, OUTPUT );
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_RESET );
        delay_us( 15 );
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9, GPIO_PIN_SET );
        pinMode( GPIO_PIN_9, INPUT );
        if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_9 )== GPIO_PIN_SET )  val = val | 0x80;
        delay_us( 45 );
    }
    return  val;
}
/*
*/

void crc8( uint8_t data)
{
	uint8_t i = 8;
	do
	{
		if( ( data ^ crcMcu ) & 1 )
		{
			crcMcu ^=0x18;
			crcMcu >>= 1;
			crcMcu |= 0x80;
		}
		else
		{
			crcMcu >>= 1;
		}
		data >>= 1;
	}
	while (--i);
}

uint16_t  ReportTemperature( void )
{
    uint32_t val;
    uint16_t t;
    uint8_t  n;
    uint8_t pad[16];

    SendInitialization();
    delay_us( 100 );
    SendByte( 0XCC );
    SendByte( 0X44 );
  
    HAL_Delay( 750 );
    SendInitialization();
    delay_us( 100 );
    SendByte( 0XCC );
    SendByte( 0XBE );
  
    for ( n = 0 ; n < 9 ; n++ )
    {
        pad[ n ] = ReadByte();
    }
    
    val = ( pad[ 1 ] * 256 + pad[ 0 ] );             // temp in 0.5 degs C
    t = val >> 4;
		
		crcSensor = pad[ 8 ];
		
		for(uint8_t i = 0; i < 8; i++)
		{
			crc8( pad[ i ] );
		}
		if( ( crcSensor ^ crcMcu ) == 0 )
		{
			crcMcu = 0;
			return t;
		}
		else
		{
			crcMcu = 0;
			return 200;
		}
}

void timeRTC( void )
{
	//Get time to the internal RTC of the STM32F401RET
	HAL_RTC_GetTime( &hrtc, &sTimeStructureGet, RTC_FORMAT_BIN );
	
	Time[ 0 ] = sTimeStructureGet.Hours;
	Time[ 1 ] = sTimeStructureGet.Minutes;
	
	timeLCD[ 8 ] = ( ( Time[0]%100 )/10 ) + 0x30;
	timeLCD[ 9 ] = ( ( Time[0]%100 )%10 ) + 0x30;
	timeLCD[ 11 ] = ( ( Time[1]%100 )/10 ) + 0x30;
	timeLCD[ 12 ] = ( ( Time[1]%100 )%10 ) + 0x30;

	disp_enviar( timeLCD, 1 );
}

void dateRTC( void )
{
	//Get the date to the internal RTC of the STM32F401RET
	HAL_RTC_GetDate( &hrtc, &sDateStructureGet, RTC_FORMAT_BIN );
	
	Date[ 0 ] = sDateStructureGet.Date;
	Date[ 1 ] = sDateStructureGet.Month;
	Date[ 2 ] = sDateStructureGet.Year;
	Date[ 3 ] = sDateStructureGet.WeekDay;
		
	dateLCD[ 7 ] = ( ( Date[0]%100 )/10 ) + 0x30;
	dateLCD[ 8 ] = ( ( Date[0]%100 )%10 ) + 0x30;
	dateLCD[ 10 ] = ( ( Date[1]%100 )/10 ) + 0x30;
	dateLCD[ 11 ] = ( ( Date[1]%100 )%10 ) + 0x30;
	dateLCD[ 13 ] = ( ( Date[2]%100 )/10 ) + 0x30;
	dateLCD[ 14 ] = ( ( Date[2]%100 )%10 ) + 0x30;
	disp_enviar( dateLCD, 0 );
}

void disp_enviar( char* text, int linea )
{
	int i,n = strlen( text );
	
	n = ( n>16 )?16:n;
	
	LCD_setCursor( linea + 1, 1 );
	
	for( i=0; i<n; i++ )
	{
		LCD_writeChar( text[ i ] );
	}
	HAL_Delay( 100 );
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
