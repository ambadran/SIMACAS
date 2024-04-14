#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define PWM_COUNTER_FREQ 100000UL
#define ULTRASONIC_COUNTER_TO_CM  233 //TODO: actually calculate and get this value
#define ULTRASONIC_TRIGGER_ON_TIME 10UL  // 10ms TODO: is this really 10us ?!

/* this status enum will be used to return whether a working ultrasonic is connected or not */
typedef enum {
  ULTRASONIC_ACTIVE,
  ULTRASONIC_INACTIVE
} ULTRASONIC_STATUS;

void ultrasonic_init(void);

ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance);

#endif
