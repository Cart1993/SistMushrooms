#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


/* Definicion de Tipos -------------------------------------------------------*/

/**
 * Estructura Basica de un Buffer circular
 */

typedef enum { QueueOK , QueueFail } QueueResp;

typedef struct {

    uint8_t* Head;              /// Primera posicion del Buffer
    uint8_t* Tail;              /// Ultima posicion del Buffer
    uint8_t* NextRead;          /// Proxima posicion a libre para escritura
    uint8_t* NextWrite;         /// Proximo elemento disponible para la lectura

    unsigned int Count;         /// Contador de data almacenada en la cola
    unsigned int Length;        /// Capacidad Maxima de la cola
    size_t ItemSize;            /// Tamaño de los elementos almacenados

} Queue_t;

typedef void* QueueHandler;

/* Public macro --------------------------------------------------------------*/

#define NewQueue( expr , Type , Len )   Type Bff_##expr[Len]; \
                                        Queue_t Queue_##expr = {.Head = (uint8_t*) Bff_##expr , \
																																.Tail =	(uint8_t*) Bff_##expr + ( Len * sizeof( Type ) ) ,\
																																.NextRead =	(uint8_t*) Bff_##expr , \
																																.NextWrite = (uint8_t*) Bff_##expr , \
																																.Count = 0 ,\
																																.Length =	Len ,\
																																.ItemSize =	sizeof(Type) };\
																				QueueHandler expr = (void*) &Queue_##expr


/* Prototipos ----------------------------------------------------------------*/


uint8_t QueueNew( QueueHandler Queue , size_t Size , void* Buff , uint16_t Len );

uint8_t QueueClear( QueueHandler Queue );

#define QueueAdd( Queue , Item ) QueueSend( Queue , (void*) &Item , 1 )

uint8_t QueueSend( QueueHandler Queue , void* const Item , uint16_t Len );

#define QueuePeek( Queue ,Item ) QueueRecive( Queue , (void*) &Item , 1 , false )
																				
#define QueueGet( Queue ,Item ) QueueRecive( Queue , (void*) &Item , 1 , true )

uint8_t QueueRecive( QueueHandler Queue , void* Item , uint16_t Len , bool Del );

unsigned int QueueCount( QueueHandler Queue );

#endif

