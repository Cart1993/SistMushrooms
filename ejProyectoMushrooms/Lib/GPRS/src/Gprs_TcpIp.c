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
#include <stdlib.h>
#include <stdio.h>

#include "Gprs_TcpIp.h"

#include "gprs_internal.h"
#include "Fibocom.h"
#include "AT.h"
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
 
char MIPCALLHandler( Command_t* _Arg );
char CmdMipopenHandler( Command_t* _Arg );
char CmdMiprtcpHandler( Command_t* _Arg );

/*------------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/
 
 static struct
 {
   char strApn[64];
   char strUsr[64];
   char strPsw[64];
 }GprsTcpIpAPN = {0};
 
 static struct
 {
   uint32_t iPortNumber;
   uint8_t iHostAddres[64];
   connect_cb connect;
   receive_cb receive;
   close_cb close;
   uint8_t * cBufferIn;
   uint8_t * cBufferInoffset;
   uint16_t iLenBuffer;
 }GprsTcpIp = {0};
 
/*------------------------------------------------------------------------------
 * Definicion de funciones Privadas
 *----------------------------------------------------------------------------*/
 
 /*
 * 0 Not registered, and the ME is not currently searching for a new operator to which to register.
 * 1 Registered, home network.
 * 2 Not registered, but the ME is currently searching for a new operator to which to register.
 * 3 Registration denied.
 * 4 Unknown.
 * 5 Registered, roaming.
 */
char CGREGHandler( Command_t* _Arg )
{
    char cAux;

    if ( _Arg->CntArg == 1 )
        cAux = _Arg->Arg[0][0];
    else
        cAux = _Arg->Arg[1][0];

    if ( cAux == '1' )
        Fibocom_Sys.Gprs = true;
    else
        Fibocom_Sys.Gprs = false;

    return 1;
}

/*----------------------------------------------------------------------------*/

 char MIPCALLHandler( Command_t* _Arg )
{
    if ( strlen( _Arg->Arg[0] ) > 1 )
    {
        strcpy( Fibocom_Sys.IPN , _Arg->Arg[0] );
        Fibocom_Sys.IP = true;
    }
    else if ( _Arg->Arg[0][0] == '1' )
        Fibocom_Sys.IP = true;
    else
        Fibocom_Sys.IP = false;

    if ( _Arg->CntArg == 2 )
        strcpy( Fibocom_Sys.IPN , _Arg->Arg[1] );

    return 1;
}

/*----------------------------------------------------------------------------*/

char CmdMipopenHandler( Command_t* _Arg )
{
  const uint8_t CntArgMipOpen = 2;
  const uint8_t ArgNumSocket = 0;
  const uint8_t ArgState = 1;

  uint8_t SocketN;
  
  if ( _Arg->CntArg == CntArgMipOpen )
  {
    SocketN = atoi(_Arg->Arg[ArgNumSocket]);

    if(_Arg->Arg[ArgState][0] == '1' || _Arg->Arg[ArgState][0] == '2')
    {
      Fibocom_Sys.Socket[SocketN - 1].Used = true;
    }
    else
    {
      Fibocom_Sys.Socket[SocketN - 1].Used = false;
    }
  }
  else if( _Arg->CntArg == 1 )
  {
    SocketN = atoi(_Arg->Arg[ArgNumSocket]);
    if(SocketN == 1 )
    {
      Fibocom_Sys.Socket[SocketN - 1].Used = true;
    }
    else if( SocketN == 0 )
    {
      Fibocom_Sys.Socket[0].Used = false;
    }
  }

  return 1;
}

/*----------------------------------------------------------------------------*/

static  uint8_t hexdigit2int(uint8_t xd)
{
  if (xd <= '9')
    return xd - '0';
  //xd = tolower(xd);  
  switch(xd)
  {
    case 'A':
      return 10;
    case 'B':
      return 11;
    case 'C':
      return 12;
    case 'D':
      return 13;
    case 'E':
      return 14;
    case 'F':
      return 15;
  }
  
  return 0;
}

char CmdMiprtcpHandler( Command_t* _Arg )
{
  const uint8_t CntArgMiprtcp = 3;
  const uint8_t ArgData = 2;

  uint16_t i , len;
  
  len = strlen(_Arg->Arg[ArgData]);
  
  if ( _Arg->CntArg == CntArgMiprtcp )
  {
    for( i = 0 ; i < (len/2) ; i++)
    {
      _Arg->Arg[ArgData][i] = (hexdigit2int(_Arg->Arg[ArgData][i*2]) << 4) + hexdigit2int(_Arg->Arg[ArgData][i*2+1]);
    }
    _Arg->Arg[ArgData][i] = 0;
    
    //hexDump("Data Recibida", _Arg->Arg[ArgData] , (len/2) );
    
    if( GprsTcpIp.receive )
    {
      GprsTcpIp.receive((uint8_t*) _Arg->Arg[ArgData] , (len/2));
    }
  }
  return 1;
}

char CmdMipStatHandler( Command_t* _Arg )
{
  const uint8_t CntArgMipstat = 2;
  const uint8_t ArgSocket = 0;
  const uint8_t ArgStat = 1;
  
  if ( _Arg->CntArg == CntArgMipstat )
  {
    if(_Arg->Arg[ArgSocket][0] == '1')
    {
      if(_Arg->Arg[ArgStat][0] == '1') 
      {
        Fibocom_Sys.Socket[0].Used = false;
        
        if( GprsTcpIp.close )
        {
          GprsTcpIp.close();
        }
      }
    }
  }
  return 1;
}

char CmdMipingstatHandler( Command_t* _Arg )
{
  return 1;
}

void error_cb( void )
{
}

/*------------------------------------------------------------------------------
 * Definicion de funciones Publicas
 *----------------------------------------------------------------------------*/


void Gprs_TcpIp_config(uint8_t *Apn , uint8_t *Usr , uint8_t *Psw)
{
  strncpy( GprsTcpIpAPN.strApn  , (char*) Apn , (sizeof GprsTcpIpAPN.strApn ) - 1 );
  strncpy( GprsTcpIpAPN.strUsr  , (char*) Usr , (sizeof GprsTcpIpAPN.strUsr ) - 1 );
  strncpy( GprsTcpIpAPN.strPsw  , (char*) Psw , (sizeof GprsTcpIpAPN.strPsw ) - 1 );
}


void Gprs_TcpIp_open( uint32_t iPortNumber , uint8_t * iHostAddress , connect_cb connect , receive_cb receive, close_cb close )
{
  GprsTcpIp.connect = connect;
  GprsTcpIp.receive = receive;
  GprsTcpIp.close = close;
  strncpy((char*) GprsTcpIp.iHostAddres ,(char*) iHostAddress , (sizeof GprsTcpIp.iHostAddres) - 1);
  GprsTcpIp.iPortNumber = iPortNumber;

  gprs_task_init(GprsTcpIp.connect , NULL );
  
  gprs_common_set_queue(gprs_get_queue(),COMMON_INIT);
  
  gprs_tcpip_set_queue(gprs_get_queue(),TCPIP_OPEN);
}

void Gprs_TcpIp_send_data( uint8_t * cBufferIn , uint16_t iLenBuffer )
{
  if(GprsTcpIp.cBufferIn)
  {
    free( GprsTcpIp.cBufferIn );
    GprsTcpIp.cBufferInoffset = NULL;
  }
  
  GprsTcpIp.cBufferIn = calloc( iLenBuffer + 1 , sizeof(uint8_t));
  memcpy( GprsTcpIp.cBufferIn , cBufferIn , iLenBuffer );
  GprsTcpIp.iLenBuffer =  iLenBuffer;
  GprsTcpIp.cBufferInoffset = GprsTcpIp.cBufferIn;
  
  gprs_task_init( NULL , error_cb );
	
	gprs_common_set_queue(gprs_get_queue(),COMMON_CMD);
	
	gprs_tcpip_set_queue(gprs_get_queue(),TCPIP_SEND);
}

void Gprs_TcpIp_close_connect( void )
{
  if(GprsTcpIp.cBufferIn)
  {
    free( GprsTcpIp.cBufferIn );
    GprsTcpIp.cBufferInoffset = NULL;
  }
  
  memset( &GprsTcpIp , 0 , sizeof GprsTcpIp);
  
  gprs_task_init( NULL , NULL );
	
	gprs_common_set_queue(gprs_get_queue(),COMMON_CMD);
	
	gprs_tcpip_set_queue(gprs_get_queue(),TCP_CLOSE);
}


 void Task_Gprs( void * _arg )
{
    GprsTask_t *pTask;

    /*Variable interna de la tarea*/
    static uint8_t TimeOut;

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        at_parser_store( "CGREG" , MIN_ESPERA , CGREGHandler);
        CGREG( "1" );
        AT_Timer_SetDelay( MIN_ESPERA );
        break;

    case EstadoInt1:
        CGREG( "?" );
        AT_Timer_SetDelay( MIN_ESPERA );
        break;

    case EstadoInt2:
        if ( Fibocom_Sys.Gprs == true )
            pTask->Status = FinOK;
        else
        {
            TimeOut = 0;
            pTask->Status = EstadoInt3;
        }
        break;

    case EstadoInt3:
        TimeOut++;
        if ( Fibocom_Sys.Gprs == true )
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

 void Task_IP( void * _arg )
{
    GprsTask_t *pTask;

    /*Variable interna de la tarea*/
    static uint8_t TimeOut;
    char Aux[3][64];

    pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        at_parser_store( "MIPCALL" , MIN_ESPERA , MIPCALLHandler);
        CSTT("\"CMNET\"");
        AT_Timer_SetDelay( 5000 );
        break;

    case EstadoInt1:
        CIICR( );
        AT_Timer_SetDelay( 5000 );
        break;

    case EstadoInt2:
        CIFSR( );
        AT_Timer_SetDelay( 5000 );
        break;

    case EstadoInt3:
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

/*----------------------------------------------------------------------------*/

void Task_Socket( void * _arg )
{
    GprsTask_t *pTask = ( GprsTask_t * ) _arg;
    char cAux[16];

    switch ( pTask->Status )
    {
    case Ini:
        at_parser_store( "MIPOPEN" , MIN_ESPERA , CmdMipopenHandler );
        at_parser_store( "MIPRTCP" , MIN_ESPERA , CmdMiprtcpHandler);
        at_parser_store( "MIPSTAT" , MIN_ESPERA , CmdMipStatHandler);
    
        Fibocom_Sys.Socket[0].Used = false;
    
        sprintf(cAux , "%d" , GprsTcpIp.iPortNumber );
        MIPOPEN( "1" , cAux , (char*) GprsTcpIp.iHostAddres  , cAux , "0");
        AT_Timer_SetTimeout( 60000 );
        break;

    case EstadoInt1:
        if ( Fibocom_Sys.Socket[0].Used == true )
        {
            pTask->Status = FinOK;
        }
        else
        {
          pTask->Status = FinError;
        }
        break;

    case EstadoInt2:
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

/*----------------------------------------------------------------------------*/

void Task_SocketSend( void * _arg )
{
    GprsTask_t *pTask = ( GprsTask_t * ) _arg;
    static uint16_t iLen;
    char cAux[16];
    const uint16_t MaxSizeSocket = 1024;

    switch ( pTask->Status )
    {
    case Ini:
        if( GprsTcpIp.cBufferIn )
        {
          if(GprsTcpIp.iLenBuffer > MaxSizeSocket)
          {
            iLen = MaxSizeSocket;
          }
          else
          {
            iLen = GprsTcpIp.iLenBuffer;
          }
          
          sprintf( cAux , "%d" , iLen );
          MIPSEND( "1" , cAux );
          AT_Timer_SetTimeout( 50000 );
        }
        else
        {
          pTask->Status = FinOK;
        }
        break;

    case EstadoInt1:
      
        Module_Write( GprsTcpIp.cBufferInoffset , iLen );
        AT_Timer_SetTimeout( 500 );
        //hexDump("Data enviada",GprsTcpIp.cBufferInoffset , iLen );
        break;
    
    case EstadoInt2:
        AT_Timer_SetTimeout( 5000 );
        /*Primer ok*/
        break;
    
    case EstadoInt3:
        GprsTcpIp.cBufferInoffset += iLen;
        GprsTcpIp.iLenBuffer -= iLen;
    
        if( GprsTcpIp.iLenBuffer )
        {
          pTask->Status = Ini;
        }
        else
        {
          if(GprsTcpIp.cBufferIn)
          {
            free( GprsTcpIp.cBufferIn );
            GprsTcpIp.cBufferInoffset = NULL;
          }
           AT_Timer_SetDelay(20000);
          pTask->Status++;
        }
        break;

    case EstadoInt4:
        
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

void Task_SocketClose( void * _arg )
{
    GprsTask_t *pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        MIPCLOSE("?");
        AT_Timer_SetTimeout( 500 );
      break;
    case EstadoInt1:
        if ( Fibocom_Sys.Socket[0].Used == false )
        {
            pTask->Status = FinOK;
        }
        else
        {
          pTask->Status++;
        }
      break;
    
    case EstadoInt2:
        MIPCLOSE("1");
        AT_Timer_SetDelay( 10000 );
        break;
    
    case EstadoInt3:
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}

void Task_ping( void * _arg )
{
    GprsTask_t *pTask = ( GprsTask_t * ) _arg;

    switch ( pTask->Status )
    {
    case Ini:
        at_parser_store( "MPINGSTAT" , MIN_ESPERA , CmdMipingstatHandler );
        MPING("www.google.com","1000");
        AT_Timer_SetTimeout( 500 );
      break;
    case EstadoInt1:
        pTask->Status++;
        AT_Timer_SetDelay(1100);
      break;
    
    case EstadoInt2:
        pTask->Status = FinOK;
        break;

    default:
        pTask->Status = FinError;
        break;
    };
}



void gprs_tcpip_set_queue(QueueHandler QueueInternal, tTCPIP_TASK eTypeTask)
{
	switch(eTypeTask)
	{
		case TCPIP_OPEN:
//			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Gprs});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_IP});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Socket});
			break;
		
		case TCPIP_SEND:
			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_SocketSend});
			break;
		
		case TCP_CLOSE:
//			QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_Gprs});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_IP});
      QueueAdd(QueueInternal,(Type_pTaskFunc_t){Task_SocketClose});
			break;
	}
}
      

/*-------------- END OF FUNCTIONS --------------------------------------------*/

