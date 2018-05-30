/**
* @file Library for Liquid Crystal Display (LCD) 16X2
*	@version 1.0
*	@date	18/11/2016
* @author Mohamed Yaqoob
*	@title Function of LCD Display 16x2
	@brief Function to connect the ARM STM32F4 to a LCD Display of 16x2
*	@description	This is a library for the standard 16X2 LCD display, for the STM32F4xx series.
								It perfroms the basic Text printing to your 16X2 LCD and is capable of operating
								in 8 bits and 4 bits modes of operation.
*
*/

/**
*	@brief Header includes
*
*/
#include "stm32f4xx_hal.h"
#include  <stdbool.h>

/**
*	@brief Constants definition
*
*/
#define ENTRY_MODE							0x04

/**
*	@brief Constants definition
*
*/
#define DISPLAY_CONTROL					0x08

/**
*	@brief Constants definition
*
*/
#define DISPLAYandCURSOR_SHIFT	0x10

/**
*	@brief Functions prototypes (private functions)
*	@brief Write command word to LCD
*
*/
static void writeCommand(uint8_t command);

/**
*	@brief Functions prototypes (private functions)
*	@brief RS Pins
*	@param uint8_t Command to write
*	@return void
*
*/
static void RS_pin(bool state);

/**
*	@brief Functions prototypes (private functions)
*	@brief E Pins
*	@param bool State of the pin RS
*	@return void
*
*/
static void E_pin(bool state);

/**
*	@brief Functions prototypes (private functions)
*	@brief Switch mode of operation
*	@param bool State of the pin E
*	@return void
*
*/
static void switchMode(bool Mode);

/**
*	@brief Functions prototypes (private functions)
*	@brief Write a single Character to the display
*	@param bool The mode of the user likes work
*	@return void
*
*/
void LCD_writeChar(char text);

/**
*	@brief Functions prototypes (private functions)
*	@brief Take the length of the received data
*	@param char Text in char format
*	@return void
*
*/
static void setDataLength(bool length);

/**
*	@brief Functions prototypes (private functions)
*	@brief Convert to string assist functions (used by itoa and ftoa)
*	@param bool Length of the received data
*	@return void
*
*/
static void reverse(char s[]);
/**
*	@brief Functions prototypes (private functions)
*	@brief Convert to string assist functions (used by itoa and ftoa) 
*	@return int Integer value
*
*/
static int intToStr(int x, char str[], int d);
/**
*	@brief Functions prototypes (private functions)
*	@brief Convert to string assist functions (used by itoa and ftoa)
*	@param *str Pointer to receive the text
*	@param len	Integer value to specified the length of the word
*	@return void
*
*/
static void reverseF(char *str, int len);



/**
*	@brief Public functions
*	@brief LCD_begin8BIT
*	@param PORT_RS_E GPIO_TypeDef that indicates the port of the Pin to control the LCD Display
*	@param RS	Pin in the MCU to control the LCD's RS Pin
*	@param E Pin in the MCU to control the LCD's E Pin
*	@param PORT_LSBs0to4 GPIO_TypeDef that indicates the port of the pins that control the LCD Display (Dates)
*	@param D0	Pin in the MCU to send the data to LCD Display
*	@param D1	Pin in the MCU to send the data to LCD Display
*	@param D2	Pin in the MCU to send the data to LCD Display
*	@param D3	Pin in the MCU to send the data to LCD Display
*	@param D4	Pin in the MCU to send the data to LCD Display
*	@param D5	Pin in the MCU to send the data to LCD Display
*	@param D6	Pin in the MCU to send the data to LCD Display
*	@param D7	Pin in the MCU to send the data to LCD Display
*	@param D8	Pin in the MCU to send the data to LCD Display
*	@return void
*
*/
void LCD_begin8BIT(GPIO_TypeDef* PORT_RS_E, uint16_t RS, uint16_t E, GPIO_TypeDef* PORT_LSBs0to4, uint16_t D0, uint16_t D1, uint16_t D2, uint16_t D3, GPIO_TypeDef* PORT_MSBs4to7, uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7);

/**
*	@brief Public functions
*	@brief LCD_begin4BIT
*	@param PORT_RS_E GPIO_TypeDef that indicates the port of the Pin to control the LCD Display
*	@param RS	Pin in the MCU to control the LCD's RS Pin
*	@param E Pin in the MCU to control the LCD's E Pin
*	@param PORT_LSBs0to4 GPIO_TypeDef that indicates the port of the pins that control the LCD Display (Dates)
*	@param D4	Pin in the MCU to send the data to LCD Display
*	@param D5	Pin in the MCU to send the data to LCD Display
*	@param D6	Pin in the MCU to send the data to LCD Display
*	@param D7	Pin in the MCU to send the data to LCD Display
*	@param D8	Pin in the MCU to send the data to LCD Display
*	@return void
*
*/
void LCD_begin4BIT(GPIO_TypeDef* PORT_RS_E, uint16_t RS, uint16_t E, GPIO_TypeDef* PORT_MSBs4to7, uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7);

/**
*	@brief Public Functions
*	@brief LCD_print Print to Display
*	@param string[]	The text to print in the LCD
*
*/
void LCD_print(char string[]);

/**
*	@brief Public Functions
*	@brief LCD_clear Clear display
*	@param void
*	@return void
*
*/
void LCD_clear(void);

/**
*	@brief Public Functions
*	@brief LCD_setCursor Set Cursor position
*	@param row Row where the cursor is positioned
*	@param col Col where the cursor is positioned
*	@return void
*
*/
void LCD_setCursor(uint8_t row, uint8_t col);

/**
*	@brief Public Functions
*	@brief LCD_TwoLines Enable two lines of the display
*	@param void
*	@return void
*
*/
void LCD_TwoLines(void);

/**
*	@brief Public Functions
*	@brief LCD_OneLine Enable two lines of the display
*	@param void
*	@return void
*
*/
void LCD_OneLine(void);

/**
*	@brief Public Functions
*	@brief LCD_noCursor Disable the function of the cursor
*	@param void
*	@return void
*
*/
void LCD_noCursor(void);

/**
*	@brief Public Functions
*	@brief LCD_noCursor Enable the function of the cursor
*	@param void
*	@return void
*
*/
void LCD_cursor(void);

/**
*	@brief Public Functions
*	@brief LCD_noBlink Don't blinking the cursor
*	@param void
*	@return void
*
*/
void LCD_noBlink(void);

/**
*	@brief Public Functions
*	@brief LCD_blink Blinking the cursor
*	@param void
*	@return void
*
*/
void LCD_blink(void);

/**
*	@brief Public Functions
*	@brief LCD_noDisplay Don't active the display on LCD
*	@param void
*	@return void
*
*/
void LCD_noDisplay(void);

/**
*	@brief Public Functions
*	@brief LCD_display Activate the display on LCD
*	@param void
*	@return void
*
*/
void LCD_display(void);

/**
*	@brief Public Functions
*	@brief LCD_shiftToRight Write the text with shift to right
*	@param num Number of positions that the shift is executed
*	@return void
*
*/
void LCD_shiftToRight(uint8_t num);

/**
*	@brief Public Functions
*	@brief LCD_shiftToLeft Write the text with shift to left
*	@param num Number of positions that the shift is executed
*	@return void
*
*/
void LCD_shiftToLeft(uint8_t num);

// Function(11): itoa and ftoa to convert from int and float to string
/**
*	@brief Public Functions
*	@brief LCD_itoa Convert from int and float to string
*	@param n Integer value to converted
*	@param	s[] Char result after the converted
*	@return void
*
*/
void LCD_itoa(int n, char s[]);
/**
*	@brief Public Functions
*	@brief LCD_itoa Convert from int and float to string
*	@param n Float value to converted
*	@param afterpoint
*	@param	*res Char result after the converted
*	@return void
*
*/
void LCD_ftoa(float n, char *res, int afterpoint);
