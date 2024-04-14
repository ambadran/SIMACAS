#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_COUNTER_TO_CM  233 //TODO: actually calculate and get this value
#define ULTRASONIC_TRIGGER_ON_TIME 10UL  // 10ms TODO: is this really 10us ?!

/* this status enum will be used to return whether a working ultrasonic is connected or not */
typedef enum {
  ULTRASONIC_ACTIVE,
  ULTRASONIC_IDLE,
  ULTRASONIC_UNRESPONSIVE
} ULTRASONIC_STATUS;

typedef enum {
  ULTRASONIC_TRIGGER_PHASE,
  ULTRASONIC_TRIGGER_SENT_PHASE,
  ULTRASONIC_
  ULTRASONIC_AWAITING_ECHO_RISE_PHASE,

} ULTRASONIC_PHASE;

bool ultrasonic_cycle_on = false; // this variable controls whether ultrasonic will be constantly working or not

void ultrasonic_init(void);
void static ultrasonic_send_trigger(void);
void static ultrasonic_await_echo(void);
void processs_ultrasonic_phases(void);
ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance);

#endif
