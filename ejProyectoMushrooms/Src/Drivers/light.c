#include "light.h"

const uint8_t limiteBajo = 85;
const uint8_t limiteMedio = 170;

extern uint32_t buffer[ 6 ];

eLight getLight( void )
{
  uint8_t ldr_1 = buffer[ 0 ];
	uint8_t ldr_2 = buffer[ 1 ];
//	uint8_t ldr_3 = buffer[ 2 ];
	uint8_t ldr_t = (ldr_1+ldr_2)/2;

  if( ldr_t < limiteBajo )
  {
    return LightHigh;
  }
  else if( ldr_t < limiteMedio )
  {
    return LightMedium;
  }

  return LightLow;
}
