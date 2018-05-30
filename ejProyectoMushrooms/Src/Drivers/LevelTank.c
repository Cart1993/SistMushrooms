#include "levelTank.h"

extern uint8_t sensorOptic;

eLevel getlevelTank()
{
  if(sensorOptic==0){
    return levelVoid;
  }else if(sensorOptic == 1){
    return levelLow;
  }else if(sensorOptic == 2){
    return levelMedium;
  }else if(sensorOptic == 3){
    return levelHigh;
  }else{
    return levelError;
  }
}
