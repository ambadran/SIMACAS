#include "project-defs.h"

uint16_t soil_moisture_last_value;

void soil_moisture_init(void) {

  adcConfigureChannel(SOIL_MOISTURE_AD_CHANNEL);

}

uint16_t soil_moisture_read(void) {

  soil_moisture_last_value = adcBlockingRead(SOIL_MOISTURE_AD_CHANNEL) * SOIL_MOISTURE_TO_KPA;
  return soil_moisture_last_value;

}

uint16_t get_soil_moisture_last_value(void) { return soil_moisture_last_value; }

