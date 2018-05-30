#ifndef _GPRS_COMMON_H_
#define _GPRS_COMMON_H_

#include <stdint.h>

/* PROTOTIPOS DE FUNCIONES PUBLICAS */

extern uint8_t gprs_common_get_signal(void);

extern uint8_t gprs_common_get_imei(uint8_t *imei);

extern uint16_t gprs_common_get_batt(void);

extern void gprs_common_update_signal(void);

extern void gprs_common_init(void);

extern void gprs_common_test(void (*Gprs_fin_ok)(void), void (*Gprs_fin_error)(void));


#endif
