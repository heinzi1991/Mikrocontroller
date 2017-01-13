#include "joystick.h"
#include "stdlib.h"
#include "stdio.h"
#include "macros.h"
#include <util/delay.h>


void joystick_init() {
  
  adc_init(0x03);

  realTimeCalibration();
    
}

int8_t calc_max_digital_x(uint16_t analog) {

  int8_t ret_max_x;
 
  ret_max_x = (0 + (((analog - (idle_analog_position.posX + 5)) * 100) / (max_analog_position.posX - (idle_analog_position.posX + 5))));
  
  return ret_max_x;
}

int8_t calc_min_digital_x(uint16_t analog) {

  int8_t ret_min_x;

  ret_min_x = (-100 + (((analog - min_analog_position.posX) * 100) / ((idle_analog_position.posX - 5) - min_analog_position.posX)));
    
  return ret_min_x;
}

int8_t calc_max_digital_y(uint16_t analog) {

  int8_t ret_max_y;
  
  ret_max_y = (0 + (((analog - (idle_analog_position.posY + 5)) * 100) / (max_analog_position.posY - (idle_analog_position.posY + 5))));

  return ret_max_y;

}

int8_t calc_min_digital_y(uint16_t analog) {

  int8_t ret_min_y;

  ret_min_y = (-100 + (((analog - min_analog_position.posY) * 100) / ((idle_analog_position.posY - 5) - min_analog_position.posY)));
  
  return ret_min_y;

}


int8_t get_digital_value_x(uint16_t analog_value) {

  int8_t ret_analog;

  if (analog_value < (idle_analog_position.posX - 5)) {

    ret_analog = calc_min_digital_x(analog_value);

  }
  else if (analog_value > (idle_analog_position.posX + 5)) {

    ret_analog = calc_max_digital_x(analog_value);

  }
  else {

    ret_analog = 0;
  }

  return ret_analog;
}

int8_t get_digital_value_y(uint16_t analog_value) {

  int8_t ret_analog;

  if (analog_value < (idle_analog_position.posY - 5)) {

    ret_analog = calc_min_digital_y(analog_value);

  }
  else if (analog_value > (idle_analog_position.posY + 5)) {

    ret_analog = calc_max_digital_y(analog_value);

  }
  else {

    ret_analog = 0;
  }

  return ret_analog;
}


Digital_position joystick_status() {

  //get x and y position of the digital value
  
  uint16_t x = adc_get(0);
  uint16_t y = adc_get(1);

  Digital_position retStruct;

  //calculation of the analog values
  
  retStruct.posX = get_digital_value_x(x);
  retStruct.posY = get_digital_value_y(y);
  
  return retStruct;
}

void realTimeCalibration() {

	max_analog_position.posX = 0;
	max_analog_position.posY = 0;

	min_analog_position.posX = 1024;
	min_analog_position.posY = 1024;


	printf("Calibration begins\n");
	printf("Push the Button for finishing calibration\n");

	while(!BUTTON) {

		uint16_t input_x = adc_get(0);
		uint16_t input_y = adc_get(1);

		if (input_x > max_analog_position.posX) {

			max_analog_position.posX = input_x;
		}

		if (input_x < min_analog_position.posX) {

			min_analog_position.posX = input_x;
		}
		
		if (input_y > max_analog_position.posY) {

			max_analog_position.posY = input_y;
		}

		if (input_y < min_analog_position.posY) {

			min_analog_position.posY = input_y;
		}
	}
	
	_delay_ms(20);

	idle_analog_position.posX = adc_get(0);
	idle_analog_position.posY = adc_get(1);

	printf("Finish Calibration\n");
	printf("The maximum values are %d for x and %d for y\n", max_analog_position.posX, max_analog_position.posY);
	printf("The minimal values are %d for x and %d for y\n", min_analog_position.posX, min_analog_position.posY);
	printf("The idle values are %d for x and %d for y\n", idle_analog_position.posX, idle_analog_position.posY);

}
