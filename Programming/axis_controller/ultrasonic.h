#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define PWM_COUNTER_FREQ 100000UL
#define ULTRASONIC_COUNTER_TO_CM   //TODO

/* this status enum will be used to return whether a working ultrasonic is connected or not */
typedef enum {
  ULTRASONIC_ACTIVE,
  ULTRASONIC_INACTIVE
} ULTRASONIC_STATUS;

void ultrasonic_init(void);

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance);

#endif
