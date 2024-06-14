#include "project-defs.h"

uint16_t mq135_last_value;

void mq135_init(void) {

  adcConfigureChannel(MQ135_AD_CHANNEL);

}

uint16_t mq135_read(void) {

  mq135_last_value = adcBlockingRead(MQ135_AD_CHANNEL) * MQ135_TO_PPM;  
  return mq135_last_value;

}


uint16_t get_mq135_last_value(void) { return mq135_last_value; }
