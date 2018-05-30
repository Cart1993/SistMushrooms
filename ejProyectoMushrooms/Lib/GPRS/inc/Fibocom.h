/* 
 * File:   Fibocom.h
 * Author: jdiaz
 *
 * Created on 10 de junio de 2014, 08:33 AM
 */

#ifndef FIBOCOM_H
#define	FIBOCOM_H

#ifdef	__cplusplus
extern "C" {
#endif
  
  #include <stdbool.h>
  #include <stdint.h>
  
  extern void fibocom_reset( void );
  extern uint8_t fibocom_bsp_init( void );
  extern bool fibocom_Get_estado( void );
  extern void fibocom_set_pwon( bool bEstado );
  extern void fibocom_on( void );
  extern void fibocom_off( void );
  extern bool Module_Write_str( char *str );
  extern bool Module_Write(uint8_t* Data,uint16_t iLen);
  extern bool Module_Read_Resp( char *_str );


#ifdef	__cplusplus
}
#endif

#endif	/* FIBOCOM_H */

