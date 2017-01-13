#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "adc.h"

typedef struct {
	
	int8_t posX;
	int8_t posY;
	
} Digital_position;


typedef struct {

  uint16_t posX;
  uint16_t posY;

} Max_analog_position;

typedef struct {

  uint16_t posX;
  uint16_t posY;

} Min_analog_position;

typedef struct {

  uint16_t posX;
  uint16_t posY;
  
} Idle_analog_position;

Max_analog_position max_analog_position;
Min_analog_position min_analog_position;
Idle_analog_position idle_analog_position;


void joystick_init();
void realTimeCalibration();
Digital_position joystick_status();

#endif