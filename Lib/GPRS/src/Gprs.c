/**
 * @file Gprs.c
 * @brief
 */

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Fibocom.h"
#include "AT.h"
#include "AT_Timer.h"
#include "Gprs.h"
#include "gprs_internal.h"


/*------------------------------------------------------------------------------
 * Definiciones
 *----------------------------------------------------------------------------*/

#define MAX_TASK_GPRS   15
#define QUEUE_LEN_GPRS	10000

/*------------------------------------------------------------------------------
 * Macros
 *----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------------------
 * Definicion de Tipos
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Prototipos Privados
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/

GprsTask_t TaskNow = {0};
GprsStatus_t Fibocom_Sys = {0};

static Gprs_fin_cb Gprs_fin_ok_cb;
static Gprs_fin_cb Gprs_fin_error_cb;

/*Lista de tareas a ser ejecutadas */
NewQueue( qTaskGprs , Type_pTaskFunc_t , MAX_TASK_GPRS );
NewQueue( RxGprs , uint8_t , QUEUE_LEN_GPRS );

/*------------------------------------------------------------------------------
 * Definicion de funciones Privadas
 *----------------------------------------------------------------------------*/

char OkHandler( Command_t* _Arg )
{
    TaskNow.Status++;
    AT_Timer_ClearTimeout( );
    return 1;
}

/*----------------------------------------------------------------------------*/

char ErrorHandler( Command_t* _Arg )
{
    TaskNow.Status = FinError;
    return 1;
}

/*----------------------------------------------------------------------------*/

char POWERHandler( Command_t* _Arg )
{
    return 1;
}

/*------------------------------------------------------------------------------
 * Definicion de funciones Publicas
 *----------------------------------------------------------------------------*/

void gprs_task_init(Gprs_fin_cb Fin_ok , Gprs_fin_cb Fin_error )
{
  QueueClear( qTaskGprs );
  TaskNow.Func = NULL;
  
  Gprs_fin_ok_cb = Fin_ok;
  Gprs_fin_error_cb = Fin_error;	

	/*se cargan los eventos deseados*/
	at_parser_store( "OK" , MIN_ESPERA , OkHandler);
	at_parser_store( "ERROR" , MIN_ESPERA , ErrorHandler);
	at_parser_store( ">" , MIN_ESPERA , OkHandler);
}

QueueHandler gprs_get_queue(void)
{
	return qTaskGprs;
}


/*------------------------------------------------
 *              ADMINISTRADOR DE TAREAS
 *------------------------------------------------*/


void gprs_add_new_data(uint8_t cByteIn)
{
	QueueAdd( RxGprs , cByteIn);
}

/**
 *
 */
void TaskScheduler( void )
{
    uint8_t Chr;
    static uint8_t Resp[3000];

    if (( TaskNow.Func == NULL ) || ( TaskNow.Status == FinOK ) )
    {
        TaskNow.Func = NULL;

        if( QueueGet( qTaskGprs , TaskNow.Func ) == true )
        {
            TaskNow.Status = Ini;
            AT_Timer_ClearTimeout();
        }
        else
        {
          //callback de finOK
          if( Gprs_fin_ok_cb != NULL )
          {
            Gprs_fin_ok_cb();
            Gprs_fin_ok_cb = NULL;
            Gprs_fin_error_cb = NULL;
          }
          return;
        }
    }

    while ( QueueGet( RxGprs , Chr ) == true )
    {
      if( AT_tramaParse(Resp,Chr) == true )
      {
        CheckATResponse((char*) Resp );
      }
    }

    if ( TaskNow.Status == FinError )
    {
        //callback de error
        if( Gprs_fin_error_cb != NULL )
        {
          Gprs_fin_error_cb();
          Gprs_fin_ok_cb = NULL;
          Gprs_fin_error_cb = NULL;
        }
        return;
    }

    if (( AT_Timer_GetTime() == 0 ) && ( TaskNow.Func != NULL ))
    {
        if ( AT_Timer_IsDelay())
        {
            TaskNow.Func(( void * ) &TaskNow );
        }
        else
        {
            TaskNow.Status = FinError;
        }
    }

    return;
}


/*-------------- END OF FUNCTIONS --------------------------------------------*/
