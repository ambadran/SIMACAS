#ifndef PROEJCT_DEFS_H
#define PROEJCT_DEFS_H

/* !!! Conversion Values !!! */
/* This MCU has 12-bit adc reading soil moisture sensor, light sensor and CO2 gas sensor 
 * All of which output ADC value 0-4096
 * This adc value must be converted to the respective unit that the sensor is measuring
 * Thus the following are the values to be multiplied with the ADC value to convert it
 * to the wanted unit 
 */
#define MQ135_TO_PPM 1  // Particles Per Million  (CO2 availability in atmosphere)
#define TEMT6000_TO_LUMENS_M2 1  // Lumens per meter squared  (light intensity)
#define SOIL_MOISTURE_TO_KPA 1  // Kilo-Pascals  (soil water tension)

/* Console settings */
#define CONSOLE_SPEED 115200
#define CONSOLE_UART UART1
#define CONSOLE_PIN_CONFIG 0

/* GPIO Pins */
#define DHT11_PORT 5
#define DHT11_PIN 4

/* AD Channels */
#define MQ135_AD_CHANNEL ADC_CHANNEL0
#define SOIL_MOISTURE_AD_CHANNEL ADC_CHANNEL1
#define TEMT6000_AD_CHANNEL ADC_CHANNEL3


#include <STC/8H8KxxU/SOP16.h>
#include <stdbool.h>
#include <stdio.h>
#include <delay.h>
#include <gpio-hal.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <spi-hal.h>
#include <adc-comp-hal.h>
#include "temt6000.h"
#include "dh11.h"
#include "soil_moisture.h"
#include "mq135.h"
#include "terminal.h"
#include "protocol.h"


#endif
