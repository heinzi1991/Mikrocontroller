#ifndef JOY2SERVO
#define JOY2SERVO

#include <stdlib.h>
#include <stdio.h>
#include "joystick.h"

typedef struct {
  
  int8_t w_1;
  int8_t w_2;

} Joystick_to_servo;

Joystick_to_servo calculate_joystick_to_servo(Digital_position digital_joystick);

#endif