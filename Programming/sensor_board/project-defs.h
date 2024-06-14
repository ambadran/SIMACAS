#ifndef PROEJCT_DEFS_H
#define PROEJCT_DEFS_H

 /* Console settings */
#define CONSOLE_SPEED 115200
#define CONSOLE_UART UART1
#define CONSOLE_PIN_CONFIG 0


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
