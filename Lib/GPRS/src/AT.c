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


/*------------------------------------------------------------------------------
 * Definiciones
 *----------------------------------------------------------------------------*/
 
 #define AT_STORAGE_SIZE 30
 #define BUFF_RX_LEN     3000
 
/*------------------------------------------------------------------------------
 * Macros
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Definicion de Tipos
 *----------------------------------------------------------------------------*/
 
 typedef enum{
  AT_Std_ESPERA = 0,
  AT_Std_DATA,
} eInerStd;
 
/*------------------------------------------------------------------------------
 * Prototipos Privados
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/

/*
 * Arreglo con las funciones que atienden
 * las posibles respuestas del modulo
 */
CmdHandler_t CmdList[AT_STORAGE_SIZE];

static uint8_t at_cmd_storage_used = 0;

/*------------------------------------------------------------------------------
 * Definicion de funciones Privadas
 *----------------------------------------------------------------------------*/

static uint32_t _AT_hash( char *cmd )
{
    uint32_t hash   = 4321;

    while(*(++cmd ))
		{
        hash = (( hash << 2 ) + hash ) + *cmd;
		}

    return hash;
}

static uint16_t _at_search( char* cmd )
{
    uint16_t i;
    uint32_t tmp_hash = _AT_hash( cmd );
 
    for( i = 0; i < at_cmd_storage_used; i++ )
        if( CmdList[ i ].hash == tmp_hash )
            return i;
 
    return 0;
}

uint8_t AT_tramaParse( uint8_t *Buff , uint8_t chr )
{
  static eInerStd Std = AT_Std_ESPERA;
  static uint16_t HeadRx = 0;
    //El protocolo AT:
    //[TX] AT+CMD=arg,arg<CR>
    //[RX] <CR><LF>ERROR<CR><LF>
    if ( Std == AT_Std_ESPERA )
    {
        HeadRx = 0;
        if ( chr == 0x0A ) //Si estando en espera recibo LF
            Std = AT_Std_DATA; //voy al estado de espera de la data

        return false;
    }
    else if ( Std == AT_Std_DATA ) //si es un byte de datos
    {
        if ( chr == 0x0A ) //en fin de trama
        {
            Buff[HeadRx] = '\0';
            Std = AT_Std_ESPERA;
            HeadRx = 0;

            return true;
        }
        else
        {
            Buff[HeadRx] = chr; //recibo el byte;
            if ( ++HeadRx == BUFF_RX_LEN) HeadRx = 0;
            Buff[HeadRx] = '\0';
        }
    }
    else
    {
      Std = AT_Std_ESPERA;
      HeadRx = 0;
      //si es una trama fuera de la secuencia, no hago nada
    }
    return false;
}

void at_parser_store( char *command , uint32_t timeout , at_cmd_cb event )
{
    CmdHandler_t cmd = {0};
 
    cmd.hash        = _AT_hash( command );
    cmd.timeout     = timeout;
    cmd.Func        = event;
    strncpy( cmd.Cmd , command , ( sizeof cmd.Cmd ) - 1 );
 
    if( strlen( command ) >= sizeof cmd.Cmd )
        return;
    
    if( at_cmd_storage_used == AT_STORAGE_SIZE )
        return;
    
    if( _at_search( command ) )
        return;
 
    CmdList[ at_cmd_storage_used ] = cmd;
    at_cmd_storage_used++;
}

/**
 * Funcion que descompone la respuesta del modulo
 * lo descompone en comandos y parametros segun la estructura AT
 * @param[in] _str  string donde se almaceno la respuesta del modulo
 * @param[out] Command_ estructora donde se almacena la descompocicion de la respuesta
 * @return <true, false>
 */
bool ParseAt( char * _str , Command_t *Command_ )
{
    char * pch;
    memset( Command_ , 0 , sizeof ( Command_t ));

    pch = strtok( _str , "+: ,\n\r" );
    while ( pch != NULL )
    {
        Command_->val[Command_->CntArg] = pch;
        Command_->CntArg++;
        pch = strtok( NULL , "+: ,\n\r" );
    }
    Command_->CntArg--;

    return true;
}

void CheckATResponse( char *_str )
{
    uint8_t i;
    uint32_t hash;
    Command_t Command = {0};

    ParseAt( _str , &Command );
    
    hash = _AT_hash( Command.Cmd );

    for ( i = 0; i < at_cmd_storage_used ; i++ )
    {
      if( hash == CmdList[i].hash )
      {
        if ( strcmp( Command.Cmd , CmdList[i].Cmd ) == 0 )
        {
            if ( CmdList[i].Func != NULL )
            {
                CmdList[i].Func( &Command );
                return;
            }
        }
      }
    }
}

/*------------------------------------------------------------------------------
 *                              COMANDOS AT
 *----------------------------------------------------------------------------*/

/*
 * Para ahorrar memoria de programa las cadenas
 * que pertenecen al protocolo se definen aparte
 */
char ATEnd[] = "\r\n";
char ATBegin[] = "AT+";

void Cmd_AT( void )
{
    Module_Write_str( "AT" );
    Module_Write_str( ATEnd );
}

void Cmd_ATI( void )
{
		Module_Write_str( "ATI" );
    Module_Write_str( ATEnd );
}

void Cmd_ATE( char* _arg )
{
		Module_Write_str( "ATE" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void Cmd_ATW( char* _arg )
{
		Module_Write_str( "AT&W" );
		Module_Write_str( _arg );
		Module_Write_str( ATEnd );
}

void Cmd_ATA( void )
{
		Module_Write_str( "ATA" );
    Module_Write_str( ATEnd );
}

void Cmd_ATH( void )
{
		Module_Write_str( "ATH" );
    Module_Write_str( ATEnd );
}

void Cmd_ATQ( char* _arg )
{
		Module_Write_str( "ATQ" );
		Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void Cmd_ATCSMINS( void )
{
	Module_Write_str( ATBegin );
	Module_Write_str( "CSMINS?" );
	Module_Write_str( ATEnd );
}

void GSV( void )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "GSV" );
		Module_Write_str( ATEnd );
}

void IPR( char* _arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "IPR" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void CPIN( char* _arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CPIN" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

/**
 *
 */
void CBC( void )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CBC" );
    Module_Write_str( ATEnd );
}

/**
 * This command displays the received signal strength
 */
void CSQ( void )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CSQ" );
    Module_Write_str( ATEnd );
}


/**
 * Comando para solicitar el IMEI
 */

void CGSN( void )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CGSN" );
		Module_Write_str( ATEnd );

}

void CCLK( char* _arg )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CCLK" );
		if ( _arg[0] != '?' )
        Module_Write_str( "=" );
		Module_Write_str( _arg );
		Module_Write_str( ATEnd );
}

void CPOWD( char* _arg )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CPOWD" );
		Module_Write_str( "=" );
		Module_Write_str( _arg );
		Module_Write_str( ATEnd );
}

void CMTE( char* _arg )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CMTE" );
		if ( _arg[0] != '?' )
        Module_Write_str( "=" );
		Module_Write_str( _arg );
		Module_Write_str( ATEnd );
}

/*------------------------------------------------
 *                  RED GSM
 *------------------------------------------------*/

/**
 * Network Registration Status
 * @param _arg Modo de funcionamiento.
 * _arg | funcion
 * -----|--------
 *  0   | Disables the network registration unsolicited result code.
 *  1   | Enables the network registration unsolicited result code +CREG: <stat>.
 *  2   | Enables the network registration and location information in unsolicited
 */
void CREG( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CREG" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void COPS( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "COPS" );
		if ( _arg[0] != '?' )
			Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}


void CBAND( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CBAND" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void CFUN( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CFUN" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}
/*------------------------------------------------
 *                      SMS
 *------------------------------------------------*/

/**
 * Preferred Message Storage
 * @param[in] _arg String identificador
 *  - "BM" Broadcast message storage
 *  - "SM" SIM message storage
 */
void CPMS( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CPMS" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=\"" );
    Module_Write_str( _arg );
    if ( _arg[0] != '?' )
        Module_Write_str( "\"" );
    Module_Write_str( ATEnd );
}

/**
 * Message Format
 * @param _arg
 * - 0 PDU mode (default)
 * - 1 Text mode
 */
void CMGF( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CMGF" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void CSCS( char *_arg )
{
		Module_Write_str( ATBegin );
    Module_Write_str( "CSCS" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=\"" );
    Module_Write_str( _arg );
		Module_Write_str( "\"" );
    Module_Write_str( ATEnd );
}

/**
 * Send SMS to Network
 * @param _arg Numero Telefonico
 */
void CMGS( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CMGS=\"" );
    Module_Write_str( _arg );
    Module_Write_str( "\"" );
    Module_Write_str( ATEnd ); 
}

void MSJ( char *_arg )
{
		Module_Write_str( _arg );
		Module_Write_str( "\x1A" );
}

/**
 * Delete Message
 * @param _Index 1-352 Index in the SMS memory of the message to be deleted.
 * @param _Delflag
 * _Delflag | funcion
 * ---------|--------
 *      0   | Deletes the message specified in <index>
 *      1   | Deletes all read messages
 *      2   | Deletes all read messages and sent MO messages
 *      3   | Deletes all read messages, sent and unsent MO messages
 *      4   | Deletes all messages
 */
void CMGD( char* _Index , char * _Delflag )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CMGD=" );
    Module_Write_str( _Index );
    if ( _Delflag != NULL )
    {
        Module_Write_str( "," );
        Module_Write_str( _Delflag );
    }
    Module_Write_str( ATEnd );
}

/**
 * These commands handle the reading of SMs.
 * The command displays the message in location <index>
 * @param _Index location of the message
 */
void CMGR( char* _Index )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CMGR=" );
    Module_Write_str( _Index );
    Module_Write_str( ATEnd );
}

/**
 * Configura la cantidad de informacion que se muestra
 * en la lectura de mensajes
 * @param _Arg
 * - 0 muestra la cantidad minima de informacion
 * - 1 muestra todos los parametros disponibles
 */
void CSDH( char* _Arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CSDH=" );
    Module_Write_str( _Arg );
    Module_Write_str( ATEnd );
}

/**
 * These commands display a list of all SMs
 * @param _Arg
 * -"SM" read messages from SIM.
 * -"" read "rec-unread" messages
 *
 */
void CMGL( char* _Arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CMGL=\"" );
    Module_Write_str( _Arg );
    Module_Write_str( ATEnd );
}

/*------------------------------------------------
 *                     GGPRS
 *------------------------------------------------*/

/**
 * GPRS Network Registration
 * @param _arg
 * - 0 Disables the network registration unsolicited result code.
 * - 1 Enables the network registration unsolicited result code +CGREG: <stat>.
 * - 2 Enables the network registration and location information in
 */
void CGREG( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CGREG" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

/**
 * GPRS Attach or Detach
 * @param _State
 * - 0 Detached.
 * - 1 Attached.
 */
void CGATT( char *_State )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CGATT" );
    if ( _State[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _State );
    Module_Write_str( ATEnd );
}

/**
 * Create a Wireless Link
 * @param _Operation
 * - 0 - disconnect a link
 * - 1 - establish GPRS link
 * @param _APN
 * @param _UserName
 * @param _Password
 */
void MIPCALL( char *_Operation , char *_APN , char *_UserName , char *_Password )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MIPCALL" );
    if ( _Operation[0] != '?' )
    {
        Module_Write_str( "=" );
        Module_Write_str( _Operation );
        Module_Write_str( ",\"" );
        Module_Write_str( _APN );
        Module_Write_str( "\",\"" );
        Module_Write_str( _UserName );
        Module_Write_str( "\",\"" );
        Module_Write_str( _Password );
        Module_Write_str( "\"" );
    }
    else
        Module_Write_str( _Operation );

    Module_Write_str( ATEnd );
}

void CSTT( char *_arg )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CSTT" );
    if ( _arg[0] != '?' )
        Module_Write_str( "=" );
    Module_Write_str( _arg );
    Module_Write_str( ATEnd );
}

void CSTT2( char *_APN ,  char *_UserName , char *_Password )
{
		Module_Write_str( ATBegin );
    Module_Write_str( "CSTT" );
    if ( _APN[0] != '?' )
		{
			Module_Write_str( "=" );
			Module_Write_str( _APN );
			Module_Write_str( ",\"" );
			Module_Write_str( _UserName );
			Module_Write_str( "\",\"" );
			Module_Write_str( _Password );
			Module_Write_str( "\"" );
		}
		else
		{
			Module_Write_str( _APN );
		}
		Module_Write_str( ATEnd );
}

void CIICR( void )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CIICR" );
    Module_Write_str( ATEnd );
}

void CIFSR( void )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "CIFSR" );
    Module_Write_str( ATEnd );
}

void CIPPING( char *HOST )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CIPPING" );
	if ( HOST[0] != '?' )
		{
			Module_Write_str( "=" );
			Module_Write_str( HOST );
		}
		else
		{
			Module_Write_str( HOST );
		}
		Module_Write_str( ATEnd );
}
/*------------------------------------------------
 *                      TCP/IP
 *------------------------------------------------*/

void SAPBR( char *cmdType, char *cid )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "SAPBR" );
		Module_Write_str( "=" );
		Module_Write_str( cmdType );
		Module_Write_str( "," );
		Module_Write_str( cid );
		Module_Write_str( ATEnd );
}


void SAPBRCONFG( char *cmdType, char *cid, char *ConParamTag, char *ConParamValue )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "SAPBR" );
		Module_Write_str( "=" );
		Module_Write_str( cmdType );
		Module_Write_str( "," );
		Module_Write_str( cid );
		Module_Write_str( ",\"" );
		Module_Write_str( ConParamTag );
		Module_Write_str( "\",\"" );
		Module_Write_str( ConParamValue );
		Module_Write_str( "\"" );
		Module_Write_str( ATEnd );
}

void CIPGSMLOC( char *type, char *cid )
{
		Module_Write_str( ATBegin );
		Module_Write_str( "CIPGSMLOC" );
		Module_Write_str( "=" );
		Module_Write_str( type );
		Module_Write_str( "," );
		Module_Write_str( cid );
		Module_Write_str( ATEnd );
}

void MIPOPEN( char* Socket_ID , char* Source_Port , char* Remote_IP , char* Remote_Port , char* Protocol )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MIPOPEN" );
    if ( Socket_ID[0] != '?' )
    {
        Module_Write_str( "=" );
        Module_Write_str( Socket_ID );
        Module_Write_str( "," );
        Module_Write_str( Source_Port );
        Module_Write_str( ",\"" );
        Module_Write_str( Remote_IP );
        Module_Write_str( "\"," );
        Module_Write_str( Remote_Port );
        Module_Write_str( "," );
        Module_Write_str( Protocol );
    }
    else
        Module_Write_str( Socket_ID );

    Module_Write_str( ATEnd );
}

void MIPSEND( char* Socket_ID , char* DataLen )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MIPSEND=" );
    Module_Write_str( Socket_ID );
    Module_Write_str( "," );
    Module_Write_str( DataLen );
    Module_Write_str( ATEnd );
}

/*
+MIPCLOSE, Close a Socket
 */
void MIPCLOSE( char* Socket_ID )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MIPCLOSE");
    if ( Socket_ID[0] != '?' )
    {
      Module_Write_str( "=" );
      Module_Write_str( Socket_ID );
    }
    else
      Module_Write_str( Socket_ID );
    
    Module_Write_str( ATEnd );
}

/**
 *Ejecuta ping a un servidor remoto
 *@param[in] host servidor destino del ping
 *@param[in] TimeOut Tiempo limite de espera en milisegundos
 */
void MPING( char* host , char* TimeOut )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MPING=1,");
    Module_Write_str(host);
    Module_Write_str(",1,,,,");
    Module_Write_str(TimeOut);
    Module_Write_str( ATEnd );
}

/**
 *Configura el tamaño del buffer de envio interno
 *@param[in] Socket_ID Identificador del Socket
 *@param[in] Socket_Size Tamaño del buffer
 */
void MIPSETS( char* Socket_ID , char* Socket_Size)
{
    Module_Write_str( ATBegin );
    Module_Write_str( "MIPSETS=" );
    Module_Write_str( Socket_ID );
    Module_Write_str( "," );
    Module_Write_str( Socket_Size );
    Module_Write_str( ATEnd );
}

/*------------------------------------------------
 *                      FTP
 *------------------------------------------------*/

/**
 * Open FTP Connection.
 * Between G600 (FTP client) and Remote FTP Server
 * @param[in] _IpUrl Url o direccion IP del servidor FTP
 * @param[in] _Username Nombre de Usuario a ser usado en la conexion FTP
 * @param[in] _Password Clave
 */
void FTPOPEN( char * _IpUrl , char * _Username , char * _Password )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPOPEN=\"" );
    Module_Write_str( _IpUrl );
    Module_Write_str( "\",\"" );
    Module_Write_str( _Username );
    Module_Write_str( "\",\"" );
    Module_Write_str( _Password );
    Module_Write_str( "\"" );
    Module_Write_str( ATEnd );
}

/**
 * Close Established FTP Connection Between G620 (FTP client) and Remote FTP Server
 */
void FTPCLOSE( void )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPCLOSE" );
    Module_Write_str( ATEnd );
}

/**
 * Change Working Directory
 * @param[in] _Directory Directorio Destino
 */
void FTPCWD( char *_Directory )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPCWD=\"" );
    Module_Write_str( _Directory );
    Module_Write_str( "\"" );
    Module_Write_str( ATEnd );
}

/**
 * Store File On Remote FTP Server.
 * @param[in] _FileName    Nombre del archivo
 * @param[in] _FileType    Tipo de archivo (0 = ASCII, 1 = Binary)
 */
void FTPSTOR( char *_FileName , char * _FileType )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPSTOR=\"" );
    Module_Write_str( _FileName );
    Module_Write_str( "\"," );
    Module_Write_str( _FileType );
    Module_Write_str( ATEnd );
}

/**
 * Descarga un archivo desde el sevidor FTP.
 * @param[in] _FileName    Nombre del archivo
 * @param[in] _FileType    Tipo de archivo (0 = ASCII, 1 = Binary)
 */
void FTPRETR( char *_FileName , char * _FileType )
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPRETR=\"" );
    Module_Write_str( _FileName );
    Module_Write_str( "\"," );
    Module_Write_str( _FileType );
    Module_Write_str( ATEnd );
}

/**
 * Borra un archivo en el servidor FTP.
 * @param _FileName Nombre del archivo
 */
void FTPDEL(char *_FileName)
{
    Module_Write_str( ATBegin );
    Module_Write_str( "FTPDEL=\"" );
    Module_Write_str( _FileName );
    Module_Write_str( "\"" );
    Module_Write_str( ATEnd );
}
