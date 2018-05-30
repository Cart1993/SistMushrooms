#ifndef _levelTank_H_
#define _levelTank_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum
{
  levelError = -1,
  levelVoid = 0,
	levelLow,
  levelMedium,
  levelHigh,
}eLevel;

eLevel getlevelTank( void );

#endif /* _levelTank_H_ */
