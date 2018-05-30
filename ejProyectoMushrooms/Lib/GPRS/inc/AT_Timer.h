#ifndef __AT_TIMER_H__
#define __AT_TIMER_H__

#include <stdbool.h>
#include <stdint.h>


void AT_Timer_init( void );
void AT_Timer_SetTimeout( uint32_t _Timeout );
void AT_Timer_ClearTimeout( void );
void AT_Timer_SetDelay( uint32_t Timeout );
uint32_t AT_Timer_GetTime( void );
bool AT_Timer_IsDelay( void );

#endif
