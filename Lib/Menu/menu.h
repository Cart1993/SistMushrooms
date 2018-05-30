/**
*	@file menu.h
* @version 1.0
*	@date 22/10/2017
* @author Jorge Luis Diaz Arias
*	@title Library menu
*	@brief Library to apply menu on LCD Display 16x2
*	@description This library is for shows a menu on the lcd display
*							 specified of the LCD 16x2.
*
*/

#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>


/**
*	@brief Constants definition
*	@brief Hour
*
*/
#define HORA                0

/**
*	@brief Constants definition
*	@brief Minutes
*
*/
#define MINUTOS             1

/**
*	@brief Constants definition
*	@brief Day
*
*/
#define DIA                 0

/**
*	@brief Constants definition
*	@brief Month
*
*/
#define MES                 1

/**
*	@brief Constants definition
*	@brief Year
*
*/
#define ANO                 2

/**
*	@brief Constants definition
*	@brief Flag
*
*/
#define FLAG                0

/**
*	@brief Constants definition
*	@brief FlagValue
*
*/
#define FLAGVALOR			1

/**
*	@brief Constants definition
*	@brief Miles
*
*/
#define MILES               0

/**
*	@brief Constants definition
*	@brief Hundreds
*
*/
#define CIENTOS             1

/**
*	@brief Constants definition
*	@brief Tens
*
*/
#define DECENAS             2

/**
*	@brief Constants definition
*	@brief Units
*
*/
#define UNIDADES            3

/**
*	@brief Struct enum MENU that contains 2 types of variable (char, char*) and 1 pointer 
*
*/
typedef struct
{
	char TipoMenu;
	char *TextoMenu;
	void(*PtrMenuSeleccion);
}MENU;

/**
*	@brief Struct enum
*
*/
enum {
    MENU_FIN,
    MENU_COMANDO,
    MENU_SUBMNU_CLAVE6,
    MENU_SUBMENU,
    SUBMENU_ANTERIOR,
    MENU_MAIN,
};

/**
*	@brief Constants definition
*	@brief Up Line
*
*/
#define LINEA_SUPERIOR  0

/**
*	@brief Constants definition
*	@brief Down Line
*
*/
#define LINEA_INFERIOR  1

/**
*	@brief Struct enum
*
*/
enum{
    TECLA_BORRAR,
    TECLA_ARRIBA,
    TECLA_ABAJO,
    TECLA_OK,
};

/**
*	@brief Function prototypes
*	@brief mnu_ejecutar_funcion Function to execute the menu with an event
*	@param unsigned char Event to execute the menu
*	@return void
*
*/
void mnu_ejecutar_funcion(unsigned char Evento);

/**
*	@brief Function prototypes
*	@brief mnu_ejecutar_funcion_periodica Function to execute the menu Periodically
*	@return void
*
*/
void mnu_ejecutar_funcion_periodica(void);

/**
*	@brief Function prototypes
*	@brief mnu_establecer_funcion_ejecutar Function to stablish the function to execute
*	@param unsigned char Pointer to function PtrNuevaFuncion with a void returned
*	@return void
*
*/
void mnu_establecer_funcion_ejecutar(void(*PtrNuevaFuncion)(unsigned char));

/**
*	@brief Function prototypes
*	@brief mnu_establecer_funcion_periodica_ejecutar Function to stablish the function to execute
*	@param unsigned char Pointer to function PtrNuevaFuncion with a void returned
*	@return void
*
*/
void mnu_establecer_funcion_periodica_ejecutar(void(*PtrNuevaFuncion)(void));

/**
*	@brief Function prototypes
*	@brief mnu_mostrar_menu Function to show the menu in that line the user prefered
*	@param MENU *Menu
*	@param int Element to choose
*	@param int Line to choose
*	@return void
*
*/
void mnu_mostrar_menu(MENU *Menu, int ElementoMenuSeleccion, int LineaElemSel);

/**
*	@brief Function prototypes
*	@brief mnu_mostrar_msj Function to show the menu in that line the user prefered
*	@param char Message 1 to show at the lcd display
*	@param char Message 2 to show at the lcd display
*	@param int Time to show it
*	@return void
*
*/
void mnu_mostrar_msj(char *mensaje1, char *mensaje2, int tiempo);

/**
*	@brief External Struct
*
*/
extern MENU MenuPrincipal[];

/**
*	@brief External Struct
*
*/
extern MENU MenuPrincipal2[];

#endif
