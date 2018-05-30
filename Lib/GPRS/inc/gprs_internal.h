#ifndef _GPRS_INTERNAL_H_
#define _GPRS_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
#include "AT.h"

/* DEFINICIONES */


#define MAX_ESPERA              60000
#define MIN_ESPERA              500

typedef enum {
    Ini = 0,
    EstadoInt1,
    EstadoInt2,
    EstadoInt3,
    EstadoInt4,
    EstadoInt5,
    EstadoInt6,
    EstadoInt7,
    EstadoInt8,
    EstadoInt9,
    FinOK,
    FinError,
    FinTimeOut
} TaskStatus_t;

/* TAREAS GPRS */

/* Tareas Comunes */

typedef enum
{
	COMMON_INIT = 0,
	COMMON_OFF,
	COMMON_SIGNAL,
	COMMON_CMD,
	COMMON_AT
}tCOMMON_TASK;


/* Tareas TcpIp */

typedef enum
{
	TCPIP_OPEN = 0,
	TCPIP_SEND,
	TCP_CLOSE
}tTCPIP_TASK;

/* ESTRUCTURAS */

typedef void (*pTaskFunc_t)( void * _arg );

typedef struct {
    pTaskFunc_t Func;
    TaskStatus_t Status;
    CmdHandler_t * CmdList;
} GprsTask_t;

typedef struct {
    uint8_t Used;
    uint8_t Port;
    char URL[32];
} Socket_t;

typedef struct {
    uint8_t Sim : 2; /*Estado de la Sim Card*/
    uint8_t Gsm : 2; /*Estado de la conexion Gsm*/
    uint8_t Gprs : 2; /*Estado de la conexion Gprs*/
    uint8_t IP : 1; /*IP asignada*/
    uint8_t Ftp : 1; /*Conexion Ftp Abierta*/
    uint8_t FtpUp : 2;
    uint8_t MaxSms;
    uint8_t NSms;
    uint8_t LSms;	
	uint8_t SignalLevel;	/* Nivel de Señal */
	uint16_t Batt; /*Battery level*/
	char IPN[16]; /*Direccion IP asignada*/
    Socket_t Socket[4]; /*Estado de los Sockets*/
	uint8_t IMEI[21];	
} GprsStatus_t;

typedef struct
{
  pTaskFunc_t pFunc;
} Type_pTaskFunc_t;

typedef void (*Gprs_fin_cb)(void);

extern GprsStatus_t Fibocom_Sys;

/* PROTOTIPOS DE FUNCIONES PUBLICAS */

extern void gprs_task_init( Gprs_fin_cb Fin_ok , Gprs_fin_cb Fin_error );

extern QueueHandler gprs_get_queue(void);

extern void gprs_common_set_queue(QueueHandler QueueInternal, tCOMMON_TASK eTypeTask);

extern void gprs_tcpip_set_queue(QueueHandler QueueInternal, tTCPIP_TASK eTypeTask);

#endif
