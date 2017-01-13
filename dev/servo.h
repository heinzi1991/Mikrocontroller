#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "adc.h"
#include "joystick.h"

void servo_init();
void servo_left(int velocity);
void servo_right(int velocity);

#endif
