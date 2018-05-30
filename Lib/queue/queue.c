/* Includes -----------------------------------------------------------------*/
#include <queue.h>

/* Private types ------------------------------------------------------------*/
/* Private constants --------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions ------------------------------------------------------- */

inline static void CommitWrite( Queue_t* pQueue )
{
    pQueue->NextWrite += pQueue->ItemSize;

    if ( pQueue->NextWrite >= pQueue->Tail )
        pQueue->NextWrite = pQueue->Head;
    
}

inline static void CommitRead( Queue_t* pQueue )
{
    pQueue->NextRead += pQueue->ItemSize;

    if ( pQueue->NextRead >= pQueue->Tail )
        pQueue->NextRead = pQueue->Head;
    
}

/* functions --------------------------------------------------------------- */

uint8_t QueueNew( QueueHandler Queue , size_t Size , void* Buff , uint16_t Len )
{
    Queue_t* pQueue = ( Queue_t* ) Queue;
    
    pQueue->ItemSize = Size;
    pQueue->Length = Len;

    pQueue->Head = Buff;
    pQueue->Tail = (uint8_t*)Buff + Len * Size; //TODO

    QueueClear( Queue );

    return 1;
}

uint8_t QueueClear( QueueHandler Queue )
{
    Queue_t* pQueue = ( Queue_t* ) Queue;
  
    if( Queue == NULL ) return 0;

    pQueue->Count = 0;
    pQueue->NextRead = pQueue->Head;
    pQueue->NextWrite = pQueue->Head;
      
    return 1;
}


unsigned int QueueCount( QueueHandler Queue )
{
	return ((Queue_t*) Queue)->Count;	
}

uint8_t QueueSend( QueueHandler Queue , void* const Item , uint16_t Len )
{
    Queue_t* pQueue = ( Queue_t* ) Queue;
		uint8_t* pItem = Item;
    uint16_t i;
    int was_masked;
		uint8_t cResp = 1; 

    if( Queue == NULL || Item == NULL || Len == 0 )
		{
			cResp = 0;
		}
		else
		{
			was_masked = __disable_irq();
		
			if(( pQueue->Count + Len  ) > pQueue->Length )
			{
				cResp = 0;
			}
			else
			{
				for( i = 0 ; i < Len ; i++ , pItem += pQueue->ItemSize )
				{
						memcpy( (char*)pQueue->NextWrite , (char*)pItem , (size_t)pQueue->ItemSize );
						CommitWrite( pQueue );
				}
				/*Codigo Protegido*/
				pQueue->Count += Len;
			}
			
			if (!was_masked)
					__enable_irq();
		}
	
    return cResp;
}

uint8_t QueueRecive( QueueHandler Queue , void* Item , uint16_t Len , bool Del )
{
    Queue_t* pQueue = ( Queue_t* ) Queue;
		uint8_t* pItem = Item;
		void* NextReadAux;
    uint16_t i;
    int was_masked;
		uint8_t cResp = 1; 
	
    if( Queue == NULL || Item == NULL || Len == 0 )
		{
			cResp = 0;
		}
		else
		{
			NextReadAux = pQueue->NextRead;
				
			was_masked = __disable_irq();
			
			if( pQueue->Count < Len )
				cResp = 0;
			else
			{
				for( i = 0 ; i < Len ; i++ , pItem += pQueue->ItemSize )
				{
						memcpy( pItem , pQueue->NextRead , pQueue->ItemSize );
						CommitRead( pQueue );
				}
				
				if ( Del == true )
				{
					/*Codigo Protegido*/
						pQueue->Count -= Len;     
				}
				else
						pQueue->NextRead = NextReadAux;
			}
			
			if (!was_masked)
					__enable_irq();
		} 
		
    return cResp;
}



