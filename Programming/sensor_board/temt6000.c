#include "project-defs.h"

uint16_t temt6000_last_value;

void temt6000_init(void) {

  adcConfigureChannel(TEMT6000_AD_CHANNEL);

}


uint16_t temt6000_read(void) {

  temt6000_last_value = adcBlockingRead(TEMT6000_AD_CHANNEL) * TEMT6000_TO_LUMENS_M2;
  return temt6000_last_value;

}


uint16_t get_temt6000_last_value(void) { return temt6000_last_value; }
