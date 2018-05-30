#include "menu.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "STM_MY_LCD16X2.h"
#include "light.h"
#include "DHT11.h"
#include "humTemp.h"
#include "soilHumidity.h"
#include "fase.h"

//******************************************************************************//
//                         DEFINICIONES GENERALES                               //
//******************************************************************************//

#define BARRIDO_CONTINUO 	// Si se quiere un efecto de barrido contnuo descomentar esta linea
#define SUBMENU_CLAVE
#define FLAGS

//**********************DECLARACION DE VARIABLES LOCALES************************//

// Usados para establecer la hora y fecha
int CampoAEstablecer = HORA;
int  auxFlag;
int  auxFlagValor;
bool confirmar = false;

char sPassword[64];
char sPassword2[64];

bool cambiar_clave;
char intento_clave;

// Usado para mostrar un mensaje por un tiempo definido en pantalla
int tiempoMensaje;

// Usado para el efecto de barrido de los elementos de un menu..
int menuElemIndex;

MENU *MenuActual = NULL;
MENU *MenuAuxClave = NULL;

int ElementoSeleccionActual = 1;
int LineaElemSeleccion = 0;
int NumeroElemMenuActual = 0;

typedef enum
{
	OFF,
	ON,
	RUN,
} eSystem;

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
	FALSE,
}eLevel;

extern eSystem system;
extern eFase date;
extern eButton msj;
extern eLevel sensorOptic;
extern uint8_t DHT11_1, DHT11_2, cntHMI2;
extern int8_t cntHMI;
extern uint16_t tempComp1, tempComp2, tempComp3, hum1, hum2;
extern eCase getHumTemp( uint8_t x );
extern void OneWire_Init(void);
extern void disp_enviar( char* text, int linea );
extern uint16_t ReportTemperature ( void );

void(*FuncionActualEjecucion)(unsigned char);

void(*FuncionPeriodica)(void);

//******************************************************************************//
//                      PROTOTIPOS DE FUNCIONES PRIVADAS                        //
//******************************************************************************//
static void mnu_mostrar_menu_pass(void);
void mnu_mostrar_menu_anterior(void);
void mnu_salir(void);

//******************************************************************************//
//                          DEFINICION DE FUNCIONES                             //
//******************************************************************************//

void mnu_dummy_function( void )
{
}

void mnu_dummy_function_ger( void )
{
	LCD_clear();
	date = fase( faseGerminacion );
	cntHMI = 0;
	system = RUN;
}

void mnu_dummy_function_pfruct( void )
{
	LCD_clear();
	date = fase( fasePreFructificacion );
	cntHMI = 0;
	system = RUN;
}

void mnu_dummy_function_indfruct( void )
{
	LCD_clear();
	date = fase( faseIndFructificacion );
	cntHMI = 0;
	system = RUN;
}

void mnu_dummy_function_plfruct( void )
{
	LCD_clear();
	date = fase( fasePlFructificacion );
	cntHMI = 0;
	system = RUN;
}

void mnu_dummy_function_restfruct( void )
{
	LCD_clear();
	date = fase( faseResFructificacion );
	cntHMI = 0;
	system = RUN;
}

void mnu_dummy_function_no( void )
{
	date = fase( faseGerminacion );
	cntHMI = 0;
	LCD_clear();
	system = RUN;
}

void mnu_HumTemp_periodica( void )
{
	if( system != OFF )
	{
		getHumTemp( DHT11_1 );
		HAL_Delay( 3000 );
		getHumTemp( DHT11_2 );
		HAL_Delay( 3000 );

    LCD_clear();
    OneWire_Init();
    tempComp1 = ReportTemperature();
		if( tempComp1 == 200 )
		{
			LCD_clear();
			HAL_Delay(100);
			disp_enviar(" ERROR DE ", 0);
			disp_enviar(" CONEXION ", 1);
			HAL_Delay(3000);
		}
		else
		{
			char dataTemp3[] = {"Temp Comp:00C"};
			dataTemp3[10] = ( ( tempComp1%100 )/10 ) + 0x30;
			dataTemp3[11] = ( ( tempComp1%100 )%10 ) + 0x30;
			disp_enviar(dataTemp3, 0);
			HAL_Delay(3000);
		}
	}
	else
	{
		mnu_salir();
		mnu_establecer_funcion_periodica_ejecutar(0);
	}
}

void mnu_dummy_function_temp(void)
{
	if( system != OFF )
	{
		LCD_clear();
		cntHMI2 = 1;
		mnu_HumTemp_periodica();
	
		mnu_establecer_funcion_periodica_ejecutar(mnu_HumTemp_periodica);
		
	}
	else
	{
		mnu_salir();
	}

}

void mnu_SoilHumidity_periodica( void )
{
	char txt0[17];
	char txt1[17];
	
	if( system != OFF )
	{
		sprintf(txt0, "    Hum1:000");
		txt0[9] = ( hum1/100 ) + 0x30;
		txt0[10] = ( ( hum1%100 )/10 ) + 0x30;
		txt0[11] = ( ( hum1%100 )%10 ) + 0x30;
		sprintf(txt1, "    Hum2:000");
		txt1[9] = ( hum2/100 ) + 0x30;
		txt1[10] = ( ( hum2%100 )/10 ) + 0x30;
		txt1[11] = ( ( hum2%100 )%10 ) + 0x30;
		disp_enviar( txt0 , 0 );
		disp_enviar( txt1 , 1 );
	}
	else
	{
		mnu_salir();
		mnu_establecer_funcion_periodica_ejecutar(0);
	}
}

void mnu_dummy_function_hum(void)
{
	if( system != OFF )
	{
		LCD_clear();
		getSoilHumidity();
		cntHMI2 = 1;
		mnu_SoilHumidity_periodica();
	
		mnu_establecer_funcion_periodica_ejecutar(mnu_SoilHumidity_periodica);
	}
	else
	{
		mnu_salir();
	}
}

void mnu_levelTank_periodica( void )
{
	char txt[17];
	
	if( system != OFF )
	{
			switch( sensorOptic )
		{
			case VOID:
				sprintf(txt,"     Vacio     ");
				break;
			case MEDIUM:
				sprintf(txt,"     Medio     ");
				break;
			case FULL:
				sprintf(txt,"     Full     ");
				break;
			default:
				sprintf(txt,"     Error     ");
				break;
		}
		disp_enviar( txt , 1 );
	}
	else
	{
		mnu_salir();
		mnu_establecer_funcion_periodica_ejecutar(0);
	}
}

void mnu_dummy_function_tank(void)
{
	if( system != OFF )
	{
		LCD_clear();
		disp_enviar("Nivel Tanque:", 0);
		cntHMI2 = 1;
		mnu_levelTank_periodica();
  
		mnu_establecer_funcion_periodica_ejecutar(mnu_levelTank_periodica);
	}else
	{
		mnu_salir();
	}
}

void mnu_light_periodica( void )
{
  char txt[17];
  
  if( system != OFF )
	{
			switch( getLight() )
		{
			case LightHigh:
				sprintf(txt,"     Alta     ");
				break;
			case LightLow:
				sprintf(txt,"     Baja     ");
				break;
			case LightMedium:
				sprintf(txt,"     Normal     ");
				break;
			default:
				sprintf(txt,"     Error     ");
				break;
		}
  
		disp_enviar( txt , 1 );
	}else
	{
		mnu_salir();
		mnu_establecer_funcion_periodica_ejecutar(0);
	}
}

void mnu_dummy_function_light(void)
{
  if( system != OFF )
	{
		LCD_clear();
		disp_enviar("Iluinacion:", 0);
		cntHMI2 = 1;
		mnu_light_periodica();
  
		mnu_establecer_funcion_periodica_ejecutar(mnu_light_periodica);
	}else
	{
		mnu_salir();
	}
}


/* ** Se carga en puntero la función que se ejecutará dentro del ciclo de espera ** */
void mnu_establecer_funcion_ejecutar(void(*PtrNuevaFuncion)(unsigned char))
{
	FuncionActualEjecucion = PtrNuevaFuncion;
}

/* ** Ejecuta función que se encuentra apuntada ** */
void mnu_ejecutar_funcion(unsigned char Evento)
{
    if(FuncionActualEjecucion != 0)
    {
        (*FuncionActualEjecucion)(Evento);
    }
}

/* ** Se carga en puntero la función que se ejecutará dentro del ciclo de espera ** */
void mnu_establecer_funcion_periodica_ejecutar(void(*PtrNuevaFuncion)(void))
{
	FuncionPeriodica = PtrNuevaFuncion;
}

/* ** Ejecuta función periodica que se encuentra apuntada ** */
void mnu_ejecutar_funcion_periodica(void)
{
	if (FuncionPeriodica != 0)
	{
		(*FuncionPeriodica)();
	}
}

void mnu_efectos_elemento_menu(void) // Efecto de barrido cuando el texto de un sub menu es mayor a 14
{
//	char texto[21];
	int len2 = strlen(MenuActual[ElementoSeleccionActual].TextoMenu);

	//memset(texto, 0x00, 21);
	//memset(texto, 0x20, pos);
	//strncat(texto, MenuActual[ElementoSeleccionActual].TextoMenu + menuElemIndex, 14);
	//len = strlen(texto);
	//memset(texto + len, 0x20, 16 - len);

	if ((menuElemIndex + 10)>len2) // Fin ejecucion
	{
		mnu_establecer_funcion_periodica_ejecutar(0);
		menuElemIndex = 0;
		mnu_mostrar_menu_anterior();
	}
	else
	{
		menuElemIndex++;
	}
}

/* ** Dibuja elementos del submenu ** */
void mnu_dibujar_elemento_menu(int NElemento)
{
	char texto[21] = { 0 };
	int NumElemAux;
	int len2;

	len2 = strlen(MenuActual[NElemento].TextoMenu); // Longitud del texto del submenu debe ser menor o igual a 14
	if (len2 >= 14)
	{
		snprintf(texto, 16 + 1, ">%s", MenuActual[NElemento].TextoMenu);
	}
	else
	{
		snprintf(texto, 16 + 1, ">%*s%*s", (16 + len2) / 2, MenuActual[NElemento].TextoMenu, (16 - len2) / 2 + 1, "");
	}

	disp_enviar(texto, LineaElemSeleccion);

	//if (len2>14)
	//{
	//	menuElemIndex = 0;
	//	mnu_establecer_funcion_periodica_ejecutar(mnu_efectos_elemento_menu);
	//}

	if (LineaElemSeleccion == 0)
	{
		if (ElementoSeleccionActual == NumeroElemMenuActual)
		{
			NumElemAux = 1;
		}
		else
		{
			NumElemAux = ElementoSeleccionActual + 1;
		}
	}
	else
	{
		if (ElementoSeleccionActual == 1)
		{
			NumElemAux = NumeroElemMenuActual;
		}
		else
		{
			NumElemAux = ElementoSeleccionActual - 1;
		}
	}

	len2 = strlen(MenuActual[NumElemAux].TextoMenu); // Longitud del texto del submenu debe ser menor o igual a 14
	memset(texto, 0x00, 21);

	if (len2 >= 14)
	{
		snprintf(texto, 16 + 1, " %s", MenuActual[NumElemAux].TextoMenu);
	}
	else
	{
		snprintf(texto, 16 + 1, " %*s%*s", (16 + len2) / 2, MenuActual[NumElemAux].TextoMenu, (16 - len2) / 2 + 1, "");
	}

	disp_enviar(texto, (LineaElemSeleccion == LINEA_SUPERIOR)? LINEA_INFERIOR: LINEA_SUPERIOR); // Texto del item no seleccionado del menu actual (en linea contraria al texto seleccionado)

}

/* ** Se cambia de elemento seleccionado en el submenu ** */
void mnu_cambio_seleccion(int NuevoElementoSeleccion)
{
	if ((NuevoElementoSeleccion >= 1) && (MenuActual[NuevoElementoSeleccion].TipoMenu != MENU_FIN) && (NuevoElementoSeleccion != ElementoSeleccionActual))
	{
		ElementoSeleccionActual = NuevoElementoSeleccion;

	}
	else if (NuevoElementoSeleccion == 0)
	{
		ElementoSeleccionActual = NumeroElemMenuActual;
		LineaElemSeleccion = LINEA_INFERIOR;
	}
	else
	{
		ElementoSeleccionActual = 1;
		LineaElemSeleccion = LINEA_SUPERIOR;
	}
	mnu_dibujar_elemento_menu(ElementoSeleccionActual); // dibuja el nuevo elemento seleccionado

}


/* ** Selecciona Menu anterior a mostrar despues de un comando ** */
void mnu_mostrar_menu_anterior(void)
{
	mnu_mostrar_menu(MenuActual, ElementoSeleccionActual, LineaElemSeleccion);
}

/* ** Atiende eventos del teclado del submenu actual ** */
void mnu_eventos_submenu(unsigned char Evento)
{
	void(*PtrComando)(void); // puntero a funcion que retorna void y no tiene parametros
	MENU *SubMenu;

	// si habia una funcion periodica relacionada con el barrido del sub menu, la cancelamos
	mnu_establecer_funcion_periodica_ejecutar(0);

    /* ** Recibe la tecla que ha sido pulsada ** */
	switch (Evento)
	{
	case TECLA_OK: // OK
				   /* ** Abrimos otro submenu o ejecutamos comando ** */
		switch (MenuActual[ElementoSeleccionActual].TipoMenu)
		{
    /* ** Recibe la tecla que ha sido pulsada ** */
        case MENU_COMANDO:
          PtrComando = (void(*)(void))MenuActual[ElementoSeleccionActual].PtrMenuSeleccion;
					if (PtrComando != 0) (*PtrComando)();
        break;
        case MENU_SUBMENU:
          SubMenu = (MENU *)MenuActual[ElementoSeleccionActual].PtrMenuSeleccion;
					mnu_mostrar_menu(SubMenu, 1, LINEA_SUPERIOR);
        break;
        case MENU_SUBMNU_CLAVE6:
					MenuAuxClave = (MENU *)MenuActual[ElementoSeleccionActual].PtrMenuSeleccion;
					mnu_mostrar_menu_pass();
				break;
			}
			break;
		/* ** Nos movemos sobre menu ** */
		case TECLA_ARRIBA:
			LineaElemSeleccion = 0;
			mnu_cambio_seleccion(ElementoSeleccionActual - 1);
		break;
		case TECLA_ABAJO:
			LineaElemSeleccion = 1;
			mnu_cambio_seleccion(ElementoSeleccionActual + 1);
		break;
	}
}

/* ** Selecciona y dibuja un Menú/SubMenu, luego carga en puntero funcion que atiende sus eventos ** */
void mnu_mostrar_menu(MENU *Menu, int ElementoMenuSeleccion, int LineaElemSel)
{
	int i;

	/* ** Guardamos el Menu actual que se visualiza y trabaja ** */
	MenuActual = Menu;
	ElementoSeleccionActual = ElementoMenuSeleccion;
	LineaElemSeleccion = LineaElemSel;  // 0= Linea arriba, 1= Linea abajo

	i = 0;
	while (MenuActual[i + 1].TipoMenu != MENU_FIN)
	{
		i++;
	}
	NumeroElemMenuActual = i;

	if (MenuActual[ElementoMenuSeleccion].TipoMenu != MENU_FIN)
	{
		mnu_dibujar_elemento_menu(ElementoMenuSeleccion);
	}

	/* ** Cargamos en puntero funcin que atiende eventos del Submenu ** */
	mnu_establecer_funcion_ejecutar(mnu_eventos_submenu);

}


#ifdef SUBMENU_CLAVE

void mnu_eventos_clave(unsigned char Evento);
void mnu_efectos_clave(void);

void mnu_dibujar_numero_clave(bool parpadea, char seleccion, char * numero)
{
	char aMensaje[21];
	char aNumero[12];
	char sAux[12];
	static bool mostrar = true;
	int i, max;

	if (parpadea)
	{
		mostrar = !mostrar;
	}

	strcpy(aMensaje, "#.: ");

	aNumero[0] = 0;

	max = strlen(sPassword);

	for (i = 0; i<max; i++)
	{
		if (i == seleccion)
		{
			if (mostrar)
			{
				sAux[0] = numero[i];
			}
			else sAux[0] = ' ';
		}
		else if ((i<seleccion) && (cambiar_clave == false))
		{
			sAux[0] = '*';
		}
		else
		{
			sAux[0] = numero[i];
		}

		sAux[1] = 0;
		strcat(aNumero, sAux);
	}

	strcat(aMensaje, aNumero);
	disp_enviar("                ", 1);
	disp_enviar(aMensaje, 1);

}

//para entrar la clave y entrar al menu correspondiente
void mnu_mostrar_menu_pass(void)
{
	CampoAEstablecer = 0;
	cambiar_clave = false;
	sPassword[0] = '-';
	sPassword[1] = '-';
	sPassword[2] = '-';
	sPassword[3] = '-';
	sPassword[4] = '-';
	sPassword[5] = '-';
	sPassword[6] = 0;

	disp_enviar("  CONTRASENA:   ", 0);

	mnu_dibujar_numero_clave(false, CampoAEstablecer, sPassword);

	mnu_establecer_funcion_ejecutar(mnu_eventos_clave);
	mnu_establecer_funcion_periodica_ejecutar(mnu_efectos_clave);
}


void mnu_procesar_clave(void)
{
	static char StoredPaswordAux[64] = "000000";

	if (cambiar_clave == false)
	{
		//Leemos la clave almacenada
		//clave_panel_leer(StoredPaswordAux);

		//solo si se entran la contrasea que es entro al menu correspondiente
		if (memcmp(StoredPaswordAux, sPassword, strlen(StoredPaswordAux)) == 0)
		{
			ElementoSeleccionActual = 1;
			LineaElemSeleccion = 0;


			mnu_mostrar_menu(MenuAuxClave, 1, LINEA_SUPERIOR);
		}
		else
		{
			mnu_mostrar_msj("Contrasea", "Incorrecta!!!", 3);
		}
	}
	else
	{
		//si las contraseas son iguales la guardo
		if (memcmp(sPassword2, sPassword, strlen(sPassword2)) == 0)
		{
		    strcpy(StoredPaswordAux,sPassword2);
			ElementoSeleccionActual = 1;
			LineaElemSeleccion = 0;
		}
		else
		{
			mnu_mostrar_msj("Las contraseas", "No son iguales", 3);
		}

	}
}

//Para cambiar la clave
void mnu_fijar_clave(void)
{
	CampoAEstablecer = 0;

	memset(sPassword, 0, sizeof(sPassword));

	sPassword[0] = '-';
	sPassword[1] = '-';
	sPassword[2] = '-';
	sPassword[3] = '-';
	sPassword[4] = '-';
	sPassword[5] = '-';

	if ((intento_clave == 0) && (cambiar_clave == false)) disp_enviar("CONTRASEA:", 0);
	if ((intento_clave == 0) && (cambiar_clave == true)) disp_enviar("NUEVA CONTRASEA", 0);
	if (intento_clave == 1) disp_enviar("CONFIRME CONT.:", 0);



	mnu_dibujar_numero_clave(false, CampoAEstablecer, sPassword);

	mnu_establecer_funcion_ejecutar(mnu_eventos_clave);
	mnu_establecer_funcion_periodica_ejecutar(mnu_efectos_clave);
}


void mnu_fijar_clave_init(void)
{
	intento_clave = 0;
	cambiar_clave = true;

	mnu_fijar_clave();
}

void mnu_eventos_confirmar_clave(unsigned char Evento)
{
	switch (Evento) {
	case TECLA_OK:		// ELEGIR ENTRE SI O NO
		if (confirmar) 	// SI SE ELIGIO SI
		{
			mnu_procesar_clave();
			mnu_mostrar_msj("La nueva contrasea", "fue programada.", 5);
		}
		else   					// SI SE ELIGIO NO
		{
			MenuActual = (MENU *)MenuPrincipal;	 	//Si no estan seguros me retorno al menu principal modo SET
			ElementoSeleccionActual = 1;
			LineaElemSeleccion = 0;
			mnu_mostrar_menu_anterior();
		}

		break;

	case TECLA_ARRIBA: // ALTERNAR ENTRE SI Y NO
	case TECLA_ABAJO:
		confirmar = !confirmar;
		if (confirmar)
		{
			disp_enviar("       SI", 1);
		}
		else
		{
			disp_enviar("       NO", 1);
		}
		break;

	}
}

void mnu_confirmar_clave(void)
{
	confirmar = false;
	disp_enviar(" Esta seguro?", 0);
	disp_enviar("       NO", 1);

	mnu_establecer_funcion_ejecutar(mnu_eventos_confirmar_clave);
}

void mnu_efectos_clave(void)  // Para el parpadeo...
{
	mnu_dibujar_numero_clave(true, CampoAEstablecer, sPassword);
}

void mnu_eventos_clave(unsigned char Evento)
{
	switch (Evento)
	{
	case TECLA_OK:
		if (sPassword[CampoAEstablecer] != '-')
		{
			CampoAEstablecer++;

			mnu_dibujar_numero_clave(false, CampoAEstablecer, sPassword);

			if (CampoAEstablecer == 6)
			{
				mnu_establecer_funcion_periodica_ejecutar(0);
				if (cambiar_clave == false) mnu_procesar_clave();
				else //estan cambiando la contrasea
				{
					if (intento_clave == 0) //es el primer intento
					{
						intento_clave++;
						sprintf(sPassword2, "%s", sPassword);
						mnu_fijar_clave();
					}
					else //es el 2do intento vamos a ver si son iguales then lo cambiamos
					{
						mnu_confirmar_clave();
					}
				}
			}
		}
		break;

	case TECLA_ARRIBA: // AUMENTAR

		if (!((sPassword[CampoAEstablecer] >= '0') && (sPassword[CampoAEstablecer] <= '9')))
			sPassword[CampoAEstablecer] = '9'; //9 para que automaticamente en la validacion de abajo se inicialice automaticamente en 0

		sPassword[CampoAEstablecer]++;

		if (sPassword[CampoAEstablecer]>'9')
		{
			sPassword[CampoAEstablecer] = '0';
		}

		mnu_dibujar_numero_clave(false, CampoAEstablecer, sPassword);
		break;
	case TECLA_ABAJO:
		if (!((sPassword[CampoAEstablecer] >= '0') && (sPassword[CampoAEstablecer] <= '9')))
			sPassword[CampoAEstablecer] = '0'; //9 para que automaticamente en la validacion de abajo se inicialice automaticamente en 0

		sPassword[CampoAEstablecer]--;

		if (sPassword[CampoAEstablecer]<'0')
		{
			sPassword[CampoAEstablecer] = '9';
		}

		mnu_dibujar_numero_clave(false, CampoAEstablecer, sPassword);


		break;

	}
}


#endif


void mnu_salir( void )
{
	LCD_clear();
	HAL_Delay(100);
	cntHMI = 0;
}

void mnu_eventos_mostrar_msj(unsigned char Evento)
{
	switch (Evento)
	{
		case TECLA_OK: // Quitar mensaje
			mnu_establecer_funcion_periodica_ejecutar(0);
      mnu_mostrar_menu_anterior();
		break;
	}
}


void mnu_efectos_mostrar_msj(void)
{
	tiempoMensaje -= 500;

	if (tiempoMensaje <= 0) // se venci el tiempo del msj en pantalla
	{
		mnu_establecer_funcion_periodica_ejecutar(0);
		mnu_mostrar_menu_anterior();
	}
}

void mnu_mostrar_msj(char *mensaje1, char *mensaje2, int tiempo) // tiempo en segundos
{
	tiempoMensaje = tiempo * 1000;

	disp_enviar(mensaje1, 0);
	disp_enviar(mensaje2, 1);

	mnu_establecer_funcion_ejecutar(mnu_eventos_mostrar_msj);
	mnu_establecer_funcion_periodica_ejecutar(mnu_efectos_mostrar_msj);
}


#ifdef  FLAGS

void mnu_guardar_flag(void)
{
	mnu_mostrar_msj("El Flag", "fue programado.", 5);
}

void mnu_eventos_confirmar_guardar_flag(unsigned char Evento)
{
	switch (Evento) {
		case TECLA_OK:		// ELEGIR ENTRE SI O NO
			if (confirmar) 	// SI SE ELIGIO SI
				{
					mnu_guardar_flag();
				}
				else   					// SI SE ELIGIO NO
					{
						mnu_mostrar_menu_anterior();
					}
		break;
		case TECLA_ARRIBA: // ALTERNAR ENTRE SI Y NO
		case TECLA_ABAJO:
			confirmar = !confirmar;
		if (confirmar)
			{
				disp_enviar("       SI", 1);
			}
			else
				{
					disp_enviar("       NO", 1);
				}
				break;
	}
}

void mnu_confirmar_guardar_flag(void)
{
	confirmar = false;
	disp_enviar(" Esta seguro?", 0);
	disp_enviar("       NO", 1);

	mnu_establecer_funcion_ejecutar(mnu_eventos_confirmar_guardar_flag);
}

void mnu_dibujar_flag(bool parpadea, char seleccion, int flag, int flagValor)
{
	char aMensaje[21];
	char aFlag[6];
	static bool mostrar = true;

	if (parpadea)
	{
		mostrar = !mostrar;
	}

	if (seleccion == FLAG)
	{
		strcpy(aMensaje, "Flag:   ");
		if (mostrar == true)
		{
			sprintf(aFlag, "%02d=%02d", flag, flagValor);
		}
		else
		{
			sprintf(aFlag, "  =%02d", flagValor);
		}
	}
	else
	{
		strcpy(aMensaje, "Valor:  ");
		if (mostrar == true)
		{
			sprintf(aFlag, "%02d=%02d", flag, flagValor);
		}
		else
		{
			sprintf(aFlag, "%02d=  ", flag);
		}
	}

	strcat(aMensaje, aFlag);
	disp_enviar(aMensaje, 1);

}

void mnu_eventos_fijar_flag(unsigned char Evento)
{
	switch (Evento)
	{
	case TECLA_OK: // FIJAR FLAG
		if (CampoAEstablecer == FLAG)
		{
			CampoAEstablecer = FLAGVALOR;
			mnu_dibujar_flag(false, CampoAEstablecer, auxFlag, auxFlagValor);
		}
		else
		{
			mnu_establecer_funcion_periodica_ejecutar(0);
			mnu_confirmar_guardar_flag();
		}
		break;

	case TECLA_ARRIBA: // AUMENTAR NUMERO DE FLAG O VALOR DE FLAG
		switch (CampoAEstablecer)
		{
		case FLAG:
			auxFlag++;
			if (auxFlag>63) auxFlag = 0; // PORQUE SEGUN EL CODIGO DE PRG_FLAG EL FLAG MAXIMO ES 63

			break;
		case FLAGVALOR:
			auxFlagValor++;
			if (auxFlagValor>99) auxFlagValor = 0;
			break;
		}
		mnu_dibujar_flag(false, CampoAEstablecer, auxFlag, auxFlagValor);
		break;
	case TECLA_ABAJO: // Alternar entre FLAG/VALOR
		switch (CampoAEstablecer)
		{
		case FLAG:
			auxFlag--;
			if (auxFlag<0) auxFlag = 63;

			break;
		case FLAGVALOR:
			auxFlagValor--;
			if (auxFlagValor<0) auxFlagValor = 99;
			break;
		}
		mnu_dibujar_flag(false, CampoAEstablecer, auxFlag, auxFlagValor);
		break;
	}
}

void mnu_efectos_fijar_flag(void)  // Para el parpadeo...
{
	mnu_dibujar_flag(true, CampoAEstablecer, auxFlag, auxFlagValor);
}

/* * PARA PROGRAMAR LOS FLAGS POR MENU    *  */
void mnu_fijar_flag(void)
{
	CampoAEstablecer = FLAG;

	auxFlag = 0;
	auxFlagValor = 0;

	disp_enviar("ESTABLECER FLAG", 0);
	disp_enviar(" ", 1); // Algo asi como disp_clear();
	mnu_dibujar_flag(false, CampoAEstablecer, auxFlag, auxFlagValor);

	mnu_establecer_funcion_ejecutar(mnu_eventos_fijar_flag);
	mnu_establecer_funcion_periodica_ejecutar(mnu_efectos_fijar_flag);

}
#endif

/*------------------------------------------------------------*/
/*Definicion de las variables del menu*/

/* ** Declaramos SubMenu para utilizar el direccionamiento ** */
MENU SubMenu1[]; // X
MENU SubMenu2[]; // Z
MENU SubMenu3[]; // Y

MENU MenuPrincipal[] =
{
	{ MENU_MAIN,			"MENU PRINCIPAL",		&mnu_salir },
	{ MENU_SUBMENU,			"Ver parametros",		&SubMenu1 },
	{ MENU_SUBMNU_CLAVE6,	"Configuraciones",		&SubMenu2 },
	{ MENU_COMANDO,			"Salir",				&mnu_salir },
	{ MENU_FIN,"",0 }
};

MENU MenuPrincipal2[] =
{
	{ MENU_MAIN,			"PERIODO CULTIVO",		&mnu_salir },
	{ MENU_SUBMENU,			" CONFIGURAR: SI ",		&SubMenu3 },
	{ MENU_COMANDO,	" CONFIGURAR: NO ", &mnu_dummy_function_no },
	{ MENU_COMANDO,			"Salir",				&mnu_salir },
	{ MENU_FIN,"",0 }
};

/* ** SubMenu:
El primer elemento direcciona al volver atras en el submenu:
SUBMENU_ANTERIOR: Regresa a submenu anterior
MenuPrincipal: Menu a donde regresar, pero en el caso de la HKA 112, se regresa haciendo OK en el sub menu regresar
*/
MENU SubMenu1[] =
{
	{ SUBMENU_ANTERIOR,		"TITULO MENU",			&MenuPrincipal },
	{ MENU_COMANDO,			"Temperatura",			&mnu_dummy_function_temp },
	{ MENU_COMANDO,			"Humedad",			    &mnu_dummy_function_hum },
	{ MENU_COMANDO,			"Nivel del Tanque",     &mnu_dummy_function_tank },
	{ MENU_COMANDO,			"Iluminacion",          &mnu_dummy_function_light },
	{ MENU_SUBMENU,			"Regresar",				&MenuPrincipal },
	{ MENU_FIN,"",0 }
};

MENU SubMenu2[] =
{
	{ SUBMENU_ANTERIOR,		"TITULO MENU",			&MenuPrincipal },
	{ MENU_COMANDO,			"Operacion 1",			&mnu_dummy_function },
	{ MENU_COMANDO,			"Configurar algo",		&mnu_fijar_flag },
	{ MENU_COMANDO,			"Cambiar Clave",		&mnu_fijar_clave_init },
	{ MENU_SUBMENU,			"Regresar",				&MenuPrincipal },
	{ MENU_FIN,"",0 }
};

MENU SubMenu3[] =
{
	{ SUBMENU_ANTERIOR,		"TITULO MENU",			&MenuPrincipal2 },
	{ MENU_COMANDO,			"1Germinacion",			&mnu_dummy_function_ger },
	{ MENU_COMANDO,			"2PFructificacion",		&mnu_dummy_function_pfruct },
	{ MENU_COMANDO,			"3IFructificacion",		&mnu_dummy_function_indfruct },
	{ MENU_COMANDO,			"4PFructificacion",		&mnu_dummy_function_plfruct },
	{ MENU_COMANDO,			"5RFructificacion",		&mnu_dummy_function_restfruct },
	{ MENU_SUBMENU,			"Regresar",				&MenuPrincipal },
	{ MENU_FIN,"",0 }
};
