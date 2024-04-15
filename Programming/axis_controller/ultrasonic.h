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
  ULTRASONIC_SEND_TRIGGER_PHASE,
  ULTRASONIC_AWAIT_TRIGGER_PHASE,
  ULTRASONIC_TRIGGER_SENT_PHASE,
  ULTRASONIC_AWAIT_ECHO_RISE_PHASE,
  ULTRASONIC_ECHO_RISE_CAPTURED_PHASE,
  ULTRASONIC_AWAIT_ECHO_FALL_PHASE,
  ULTRASONIC_ECHO_FALL_CAPTURED_PHASE
} ULTRASONIC_PHASE;


void ultrasonic_init(void);
void ultrasonic_start_sequence(void); // sets global flag
void ultrasonic_stop_sequence(void);  // resets _state and clear global flag
void static ultrasonic_send_trigger(void);
void static ultrasonic_await_echo_rise(void);
void static ultrasonic_await_echo_fall(void);
void processs_ultrasonic_phases(void);
ULTRASONIC_STATUS ultrasonic_get_distance(uint16_t* distance);

#endif
