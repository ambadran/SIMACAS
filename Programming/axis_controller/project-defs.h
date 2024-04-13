#ifndef PROJECT_DEFS_H
#define PROJECT_DEFS_H

 /* SPI settings */
#define SPI_PIN_CONFIG 0
#define SPI_SPEED 4000000UL

 /* Console settings */
#define CONSOLE_SPEED 115200
#define CONSOLE_UART UART1
#define CONSOLE_PIN_CONFIG 0
// uncomment to echo any character written and char count in terminal
#define ECHO_TERMINAL 

 /* nRF24L01 settings */
//uncomment to print all the nrf24l01 registers after setting them with nrf24_device()
#define PRINT_NRF24_REGISTERS 

/* Stepper Motor GPIO Pins */
#define STEPPER_MOTOR_PORT 2
#define STEPPER_ENABLE_PIN 3
#define STEPPER_DIR2_PIN 2
#define STEPPER_STEP_PIN 1
#define STEPPER_DIR1_PIN 0
#define DEFAULT_STEPPER_FREQUENCY 50
#define STEPPER_TIMER TIMER2
#define STEPPER_TIMER_ISR timer2_isr
#define STEPPER_TIMER_INTERRUPT TIMER2_INTERRUPT
#define STEPPER_CM_TO_STEPS 100 //TODO: needs testing

/* timer-hal configs */
/* #define HAL_TIMER_API_STOP_TIMER */ // if set_frequency doesn't work, try stopTimer then startTimer (restarting it)

// Others 
#define SUPPRESS_delay1us_WARNING

#include <STC/8H8KxxU/SKDIP28.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <delay.h>
#include <gpio-hal.h>
#include <spi-hal.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>
#include <timer-hal.h>
#include "stepper_motor.h"
#include "nrf24l01.h"
#include "terminal.h"
#include "protocol.h"

#endif
