/**
 * @file AT.c
 * @brief 
 */
 
/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
 
#include <string.h>
#include <stdlib.h>
#include "AT.h"
#include "Fibocom.h"
#include "gprs_internal.h"
#include "AT_Timer.h"

/*------------------------------------------------------------------------------
 * Definiciones
 *----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------------------
 * Macros
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Definicion de Tipos
 *----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------------------
 * Prototipos Privados
 *----------------------------------------------------------------------------*/
 
char OkHandler( Command_t* _Arg );
char ErrorHandler( Command_t* _Arg );
char CBCHandler( Command_t* _Arg );
char CSQHandler( Command_t* _Arg );
char CPINHandler( Command_t* _Arg );
char CREGHandler( Command_t* _Arg );
char CGREGHandler( Command_t* _Arg );
char POWERHandler( Command_t* _Arg );
 
/*------------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------------------
 * Definicion de funciones Privadas
 *----------------------------------------------------------------------------*/

char CBCHandler( Command_t* _Arg )
{
			int Batt;

			Batt = atoi( _Arg->Arg[1] );
//    //El Datasheet del modulo indica que el rango de voltages es de 3,3 a 4,5
//    //Se coloca el limite de voltage en 3,5 para garantizar el envio completo
//    if ( Batt < 3500 )
//        TaskNow.Status = FinError; //Se declara el error por bajo voltage
			Fibocom_Sys.Batt=Batt;
    return 1;
}

/*----------------------------------------------------------------------------*/

/*
 * +CSQ: <rssi>,<ber>
 */

char CSQHandler( Command_t* _Arg )
{
	Fibocom_Sys.SignalLevel = 0;
	
	Fibocom_Sys.SignalLevel = atoi((char *)_Arg->Arg[0]);
	
  return 1;
}

char CGSNHandler( Command_t* _Arg )
{
	if ( _Arg->CntArg >= 1 )
	{
		sscanf( _Arg->Arg[0] , "\"%20[^\"]\"" , (char*)Fibocom_Sys.IMEI);
	}
	
	return 1;
}

/*----------------------------------------------------------------------------*/

char CPINHandler( Command_t* _Arg )
{
    if ( strcmp( _Arg->Arg[0] , "READY" ) == 0 )
        Fibocom_Sys.Sim = true;
    else
        Fibocom_Sys.Sim = false;
    AT_Timer_ClearTimeout( );
    return 1;
}

/*----------------------------------------------------------------------------*/

/*
 * +CREG: <n>,<stat>
 * 0 Not registered, and the ME is not currently searching for a new operator to which to register.
 * 1 Registered, home network.
 * 2 Not registered, but the ME is currently searching for a new operator to which to register.
 * 3 Registration denied.
 * 4 Unknown.
 * 5 Registered, roaming.
 */
char CREGHandler( Command_t* _Arg )
{
    char cAux;

    if ( _Arg->CntArg == 1 )
        cAux = _Arg->Arg[0][0];
    else
        cAux = _Arg->Arg[1][0];

    if ( cAux == '1' )
        Fibocom_Sys.Gsm = true;
    else
        Fibocom_Sys.Gsm = false;

    return 1;
}

/*------------------------------------------------------------------------------
 * Definicion de funciones Publicas
 *----------------------------------------------------------------------------*/
 
void gprs_common_task()
{
	
}



void Task_On( void * _arg )
{
    GprsTask_t *pTask;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {

    case Ini:
        if ( fibocom_Get_estado( ) == false )
            pTask->Status = EstadoInt1;
        else
            pTask->Status = FinOK;
        break;

    case EstadoInt1:
        fibocom_set_pwon( 0 );
        AT_Timer_SetDelay( 800 );
        pTask->Status = EstadoInt2;
        break;

    case EstadoInt2:
        fibocom_set_pwon( 1 );
        AT_Timer_SetDelay( 500 );
        pTask->Status = Ini;
        break;

    default:
        pTask->Status = FinError;
        break;
    }
}

/*----------------------------------------------------------------------------*/

void Task_AT( void * _arg )
{
    GprsTask_t *pTask;
    static char CntReint;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        CntReint = 0;
        AT_Timer_SetDelay( 1000 );
        pTask->Status++;
        break;

    case EstadoInt1:
        CntReint++;
        if ( CntReint >= 4 )
            pTask->Status = FinError;
        else
            Cmd_AT( );
        AT_Timer_SetDelay( 500 );
        break;

    case EstadoInt2:
        Cmd_ATE( );

        AT_Timer_SetDelay( 500 );
        break;
    case EstadoInt3:
        CFUN("?");

        AT_Timer_SetDelay( 500 );
        break;
    case EstadoInt4:
        CFUN("1");
        AT_Timer_SetDelay( 500 );
        break;
    case EstadoInt5:
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

/*----------------------------------------------------------------------------*/

void Task_Gsm( void * _arg )
{
    GprsTask_t *pTask;

    /*Variable interna de la tarea*/
    static uint8_t TimeOut;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        at_parser_store( "CREG" , MIN_ESPERA , CREGHandler );
        CREG( "1" );
        AT_Timer_SetDelay( MIN_ESPERA );
        break;

    case EstadoInt1:
        CREG( "?" );
        AT_Timer_SetDelay( MIN_ESPERA );
        break;

    case EstadoInt2:
        if ( Fibocom_Sys.Gsm == true )
            pTask->Status = FinOK;
        else
        {
            TimeOut = 0;
            pTask->Status = EstadoInt3;
        }
        break;

    case EstadoInt3:
        TimeOut++;
        if ( Fibocom_Sys.Gsm == true )
            pTask->Status = FinOK;
        else
            if ( TimeOut > 60 )
            pTask->Status = FinError;
        AT_Timer_SetDelay( 1000 );
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

/*----------------------------------------------------------------------------*/

void Task_Off( void * _arg )
{
    GprsTask_t *pTask;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        memset( ( void* ) &Fibocom_Sys , 0 , sizeof ( Fibocom_Sys ) );
        pTask->Status++;
        break;

    case EstadoInt1:
        fibocom_set_pwon( 1 );
        AT_Timer_SetDelay( 10 );
        pTask->Status++;
        break;

    case EstadoInt2:
        fibocom_set_pwon( 0 );
        AT_Timer_SetDelay( 3200 );
        pTask->Status++;
        break;

    case EstadoInt3:
        fibocom_set_pwon( 1 );
        AT_Timer_SetDelay( 500 );
        pTask->Status++;
        break;

    case EstadoInt4:
        if ( fibocom_Get_estado( ) == 1 )
            pTask->Status = EstadoInt1;
        else
        {
            pTask->Status = FinOK;
        }
        break;

    default:
        pTask->Status = FinError;
        break;
    }
}

/*----------------------------------------------------------------------------*/

void Task_GetInfo( void * _arg )
{
    GprsTask_t *pTask;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
      // Lectura del nivel de carga de la bateria
      case Ini:
          at_parser_store( "CBC" , MIN_ESPERA , CBCHandler);
          CBC( );
          AT_Timer_SetDelay( 500 );
          break;

      //Lectura de la intensidad de la señal
      case EstadoInt1:
          at_parser_store( "CSQ" , MIN_ESPERA , CSQHandler);
          CSQ( );
          AT_Timer_SetDelay( 500 );
          break;
      
      //Lectura del imei del modulo
      case EstadoInt2:
          at_parser_store( "CGSN" , MIN_ESPERA , CGSNHandler);
          CGSN();
          AT_Timer_SetDelay( 500 );
          break;

      case EstadoInt3:
          pTask->Status = FinOK;
          break;

      default:
          pTask->Status = FinError;
          break;
    }
}


void gprs_common_set_queue(QueueHandler QueueInternal, tCOMMON_TASK eTypeTask)
{
	switch(eTypeTask)
	{
		case COMMON_INIT:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_On});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_AT});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Gsm});
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_GetInfo});
			break;
		
		case COMMON_OFF:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Off});
			break;
		
		case COMMON_SIGNAL:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_AT});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_GetInfo});
			break;
			
		case COMMON_CMD:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_AT});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Gsm});
			break;
			
		case COMMON_AT:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_AT});
			break;
	}
}

/**
  * Función para obtener el Nivel de Señal  
  * 
  * @param void
  * @return void
  * 
  */


uint8_t gprs_common_get_signal(void)
{
	return Fibocom_Sys.SignalLevel;
}

/**
  * Función para obtener el nivel de la bateria 
  * 
  * @param void
  * @return void
  * 
  */

uint16_t gprs_common_get_batt(void)
{
	return Fibocom_Sys.Batt;
}

/**
  * Función para obtener el IMEI del modulo GPRS 
  * 
  * @param void
  * @return void
  * 
  */

uint8_t gprs_common_get_imei(uint8_t *imei)
{	
	strncpy((char*) imei , (char*) Fibocom_Sys.IMEI , sizeof(Fibocom_Sys.IMEI));
	return true;
}


void gprs_common_update_signal(void)
{
	gprs_task_init( NULL , NULL );
	
	gprs_common_set_queue(gprs_get_queue(),COMMON_SIGNAL);
}

void gprs_common_test(void (*Gprs_fin_ok)(void), void (*Gprs_fin_error)(void))
{
	gprs_task_init( Gprs_fin_ok , Gprs_fin_error );
	
	gprs_common_set_queue(gprs_get_queue(),COMMON_AT);
}

void gprs_common_init(void)
{
	gprs_task_init( NULL , NULL );
	
	gprs_common_set_queue(gprs_get_queue(),COMMON_INIT);
}


/*-------------- END OF FUNCTIONS --------------------------------------------*/
