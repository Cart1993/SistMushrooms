#ifndef __GPRS_H__
#define __GPRS_H__

#include <stdint.h>
#include <stdbool.h>

extern void gprs_add_new_data(uint8_t cByteIn);
extern void TaskScheduler( void );

#endif
