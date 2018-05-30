/**
 * @file at_timer.c
 * @brief AT Timer
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "AT_Timer.h"

/**/
#include "stm32f4xx_hal.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static volatile struct
{
  uint32_t TickStart;
  uint32_t TimeWait;
  bool Delay;
} tAtTimer;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Public Function Definitions
*******************************************************************************/

void AT_Timer_init(void)
{
    tAtTimer.TickStart = 0;
    tAtTimer.TimeWait = 0;
    tAtTimer.Delay = false;
}


/**
 * Funcion que setea el timeout de una tarea
 * @param _Timeout Tiempo maximo de espera
 */
void AT_Timer_SetTimeout( uint32_t Timeout )
{
    tAtTimer.TickStart = HAL_GetTick();
    tAtTimer.TimeWait = Timeout;
    tAtTimer.Delay = false;
}

/*Borra el timeout*/
void AT_Timer_ClearTimeout( void )
{
    tAtTimer.TimeWait = 0;
    tAtTimer.Delay = true;
}

/*Funcion que setea un delay entre estados internos*/
void AT_Timer_SetDelay( uint32_t Timeout )
{
    tAtTimer.TickStart = HAL_GetTick();
    tAtTimer.TimeWait = Timeout;
    tAtTimer.Delay = true;
}

uint32_t AT_Timer_GetTime( void )
{
  uint32_t lAux = HAL_GetTick() - tAtTimer.TickStart;
  
  if( lAux < tAtTimer.TimeWait )
  {
    return lAux + 1;
  }
  
  return 0;
}

bool AT_Timer_IsDelay( void )
{
  return tAtTimer.Delay;
}

/*************** END OF FUNCTIONS *********************************************/
