#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#define DIRECTION_BACKWARD 0
#define DIRECTION_FORWARD 1

void stepper_motor_move(uint8_t direction, float distance);

#endif
