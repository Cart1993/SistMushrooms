#ifndef AT_H
#define	AT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#pragma anon_unions

/**
*	@brief Union para comando enviado, argumentos recibidos y contador
*
*/

typedef union
{
    struct{
    char *Cmd;			/* Comando enviado */
    char *Arg[15];	/* Argumentos recibidos */
    char CntArg;
    };
    char *val[16];
}Command_t;

typedef char ( *at_cmd_cb )( Command_t *response );

/**
*	@brief Estructura que guarda un arreglo char para los comandos, una variable de 32 bits para el timeout y el hash
*
*/

typedef struct {
    char Cmd[16];
    uint32_t hash;
    uint32_t timeout;
    at_cmd_cb Func;
}CmdHandler_t;

/**
*	@brief ParseAt realiza el parseo de los comandos AT
*	@param _str Donde se almacena la respuesta del modulo
*	@param Command Estructora donde se almacena la descompocicion de la respuesta
*	@return bool Valor booleano
*
*/

extern bool ParseAt(char * _str, Command_t *_Command);

/**
*	@brief CheckATResponse revisa la respuesta AT
*	@param _str Donde se almacena la respuesta del modulo
*	@return void Vacio
*
*/

extern void CheckATResponse(char *_str);

/**
*	@brief at_parser_store almacena el parseo de los comandos AT
*	@param command comando
*	@param timeout tiempo de espera
*	@return void Vacio
*
*/

extern void at_parser_store( char *command , uint32_t timeout , at_cmd_cb event );

/**
*	@brief AT_tramaParse Parseo de trama de los comandos AT
*	@param Buff Buffer
*	@param chr caracter
*	@return uint8_t Entero sin signo de 8 bits
*
*/

extern uint8_t AT_tramaParse( uint8_t *Buff , uint8_t chr );

/*COMANDOS AT*/

/**
*	@brief Cmd_AT Envío de comando AT
*	@param void Vacio
*	@return void Vacio
*
*/

extern void Cmd_AT( void );

/**
*	@brief Cmd_ATI Envío de comando ATI (Display Product Identification Information)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void Cmd_ATI( void );

/**
*	@brief Cmd_ATE Envío de comando ATE (Set Command Echo Mode)
*	@param _arg Argumento del comando ( 0---> Echo Off. 1---> Echo On )
*	@return void Vacio
*
*/

extern void Cmd_ATE( char* _arg );

/**
*	@brief Cmd_ATW Envío de comando ATW
*	@param _arg Argumento del comando
*	@return void Vacio
*
*/

extern void Cmd_ATW( char* _arg );

extern void Cmd_ATA( void );

extern void Cmd_ATH( void );

extern void Cmd_ATCSMINS( void );

extern void Cmd_ATQ( char* _arg);

/**
*	@brief GSV Envío de comando AT+GSV (Display Product Identification Information)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void GSV( void );

/**
*	@brief IPR Envío de comando AT+IPR (Set TE-TA Fixed Local Rate)
*	@param _arg Argumento del comando (Baud rate)
*	@return void Vacio
*
*/

extern void IPR( char* _arg );

/**
*	@brief CPIN Envío de comando AT+CPIN (Enter PIN of the SIM)
*	@param _arg Argumento del comando (PIN)
*	@return void Vacio
*
*/

extern void CPIN( char* _arg );

/**
*	@brief CBC Envío de comando AT+CBC (Battery charge)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void CBC( void );

/**
*	@brief CSQ Envío de comando AT+CSQ (Signal quality report)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void CSQ( void );

/**
*	@brief CGSN Envío de comando AT+CGSN (Request Product Serial Number Identification)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void CGSN( void );

/**
*	@brief CCLK Envío de comando AT+CCLK (Clock of the module)
*	@param _arg Argumento del comando
*	@return void Vacio
*
*/

extern void CCLK( char* _arg );

/**
*	@brief CPOWD Envío de comando AT+CPOWD (Power Off the module)
*	@param _arg Argumento del comando (0---> Power Off urgently, 1---> Normal Power Off)
*	@return void Vacio
*
*/

extern void CPOWD( char* _arg );

/**
*	@brief CMTE Envío de comando AT+CMTE (Set critical temperature operating mode or query temperature)
*	@param _arg Argumento del comando (0---> Disable detection, 1---> Enable detection)
*	@return void Vacio
*
*/

extern void CMTE( char* _arg );

/*RED GSM*/

/**
*	@brief CREG Envío de comando AT+CREG (Network Registration)
*	@param _arg Argumento del comando (0---> Disable registratio, 1---> Enable registration, 2---> Enable registration with location information)
*	@return void Vacio
*
*/

extern void CREG( char *_arg );

/**
*	@brief COPS Envío de comando AT+COPS (Operator selection)
*	@param _arg Argumento del comando
*	@return void Vacio
*
*/

extern void COPS( char *_arg );

/**
*	@brief CBAND Envío de comando AT+CBAND (Get and set mobile operation band)
*	@param _arg Argumento del comando (A string parameter which indicate the operation band)
*	@return void Vacio
*
*/

extern void CBAND( char *_arg );

/**
*	@brief CFUN Envío de comando AT+CFUN (Set Phone Functionality)
*	@param _arg Argumento del comando (0---> Minimum Functionality, 1---> Full Functionality, 4---> Flight Mode)
*	@return void Vacio
*
*/

extern void CFUN( char *_arg );

/*SMS*/

/**
*	@brief CPMS Envío de comando AT+CPMS (Preferred SMS message storage)
*	@param _arg Argumento del comando (mem1, mem2, mem3)
*	@return void Vacio
*
*/

extern void CPMS( char *_arg );

/**
*	@brief CMGF Envío de comando AT+CMGF (Select SMS message format)
*	@param _arg Argumento del comando (0---> PDU mode, 1---> Text mode)
*	@return void Vacio
*
*/

extern void CMGF( char *_arg );

/**
*	@brief CSCS Envío de comando AT+CSCS (Select TE character Set)
*	@param _arg Argumento del comando
*	@return void Vacio
*
*/

extern void CSCS( char *_arg );

/**
*	@brief CMGS Envío de comando AT+CMGS (Send SMS message)
*	@param _arg Argumento del comando (Mensaje)
*	@return void Vacio
*
*/

extern void CMGS( char *_arg );

/**
*	@brief MSJ Envío del mensaje
*	@param _arg Mensaje
*	@return void Vacio
*
*/

extern void MSJ( char *_arg );

/**
*	@brief CMGD Envío de comando AT+CMGD (Delete SMS message)
*	@param _Index Indice que indica que mensaje sera eliminado
*	@param _Delflag Eliminar mensaje (especificado por el indice, los leidos, los no leidos, todos)
*	@return void Vacio
*
*/

extern void CMGD( char* _Index,char * _Delflag);

/**
*	@brief CMGR Envío de comando AT+CMGR (Read SMS message)
*	@param _Index Indice que indica que mensaje sera leido
*	@return void Vacio
*
*/

extern void CMGR( char* _Index );

/**
*	@brief CSDH Envío de comando AT+CSDH (Show SMS text mode parameters)
*	@param _Arg Argumento del comando (0---> Don't show, 1---> Show the values)
*	@return void Vacio
*
*/

extern void CSDH( char* _Arg );

/**
*	@brief CMGL Envío de comando AT+CSDH (List SMS messages from preferred store)
*	@param _Arg Argumento del comando
*	@return void Vacio
*
*/

extern void CMGL( char* _Arg );

/*GPRS*/

/**
*	@brief CGREG Envío de comando AT+CGREG (Network registration status)
*	@param _arg Argumento del comando (0---> Disable network registration, 1---> Enable network registration, 2---> Enable network registration with location information)
*	@return void Vacio
*
*/

extern void CGREG(char *_arg);

/**
*	@brief CGATT Envío de comando AT+CGATT (Attach or Dettach from GPRS service)
*	@param _State (0---> Detached, 1--->Attached)
*	@return void Vacio
*
*/

extern void CGATT(char *_State);
extern void MIPCALL( char *_Operation , char *_APN ,  char *_UserName , char *_Password );

/**
*	@brief CSTT Envío de comando AT+CSTT (Start task and set APN, user name, password)
*	@param _arg Argumento del comando
*	@return void Vacio
*
*/

extern void CSTT( char *_arg );

/**
*	@brief CSTT Envío de comando AT+CSTT (Start task and set APN, user name, password)
*	@param _APN APN del servicio de red movil
*	@param _UserName nombre de usuario del servicio de red movil
*	@param _Password contraseña del servicio de red movil
*	@return void Vacio
*
*/

extern void CSTT2( char *_APN ,  char *_UserName , char *_Password );

/**
*	@brief CIICR Envío de comando AT+CIICR (Bring up Wireless connection with GPRS or CSD)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void CIICR( void );

/**
*	@brief CIFSR Envío de comando AT+CIFSR (Get local IP Address)
*	@param void Vacio
*	@return void Vacio
*
*/

extern void CIFSR( void );

/**
*	@brief CIPPING Envío de comando AT+CIPPING (Ping Request)
*	@param HOST Servidor
*	@return void Vacio
*
*/

extern void CIPPING( char *HOST );

/*FTP*/
extern void FTPOPEN( char * _IpUrl , char * _Username , char * _Password );
extern void FTPCLOSE( void );
extern void FTPCWD(char *_Directory);
extern void FTPSTOR( char *_FileName , char * _FileType );
extern void FTPRETR( char *_FileName , char * _FileType );
extern void FTPDEL(char *_FileName);

/*TCP-IP*/

/**
*	@brief SAPBR Envío de comando AT+SAPBR (Bearer settings for applications based on IP)
*	@param cmdType Configuracion del bearer
*	@param cid Identificador del perfil bearer
*	@return void Vacio
*
*/

extern void SAPBR( char *cmdType, char *cid );

/**
*	@brief SAPBRCONFIG Envío de comando AT+SAPBR (Bearer settings for applications based on IP)
*	@param cmdType Configuracion del bearer
*	@param cid Identificador del perfil bearer
*	@param ConParamTag Parametro del bearer
*	@param ConParamValue Parametro de valor del bearer
*	@return void Vacio
*
*/

extern void SAPBRCONFG( char *cmdType, char *cid, char *ConParamTag, char *ConParamValue );

/**
*	@brief CIPGSMLOC Envío de comando AT+CIPGSMLOC (GSM Location and Time)
*	@param cmdType Configuracion del bearer
*	@param cid Identificador del perfil bearer
*	@return void Vacio
*
*/

extern void CIPGSMLOC( char *type, char *cid );
extern void MIPOPEN( char* Socket_ID , char* Source_Port , char* Remote_IP , char* Remote_Port , char* Protocol );
extern void MIPSEND( char* Socket_ID , char* DataLen );
extern void MIPCLOSE( char* Socket_ID );
extern void MIPSETS( char* Socket_ID , char* Socket_Size);
extern void MPING( char* host , char* TimeOut );


#ifdef	__cplusplus
}
#endif

#endif	/* AT_H */

