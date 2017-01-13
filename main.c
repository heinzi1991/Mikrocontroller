/*
 * main.c
 *
 *  Created on: 18/mar/2014
 *      Author: lasagni
 */

#include <stdio.h>
#include "dev/uart.h"
#include "dev/joystick.h"
#include "dev/servo.h"
#include "dev/joy2servo.h"
#include "dev/macros.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


volatile Digital_position joystick_state;
volatile int collision_detected; 


void let_leds_blinking() {

	cli();

	//normal mode
	TCCR1A |= (1 << WGM12);

	//prescale 64 bit because of math
	TCCR1B |= (1 << CS11) | (1 << CS10);

	//Allow compare Interrupt 
	TIMSK1 |= (1 << OCIE1A);
	
	OCR1A = 0xC350;

	sei();

}

void collision_button() {

	cli();

	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT5);

	sei();
}


ISR(TIMER1_COMPA_vect) {

	if (joystick_state.posX > 50) {

		PORTE ^= YELLOW_LED;
		PORTE &= ~(RED_LED);

	} else if (joystick_state.posX < -50) {

		PORTE ^= RED_LED;
		PORTE &= ~(YELLOW_LED);

	} else if (joystick_state.posY > 0 || joystick_state.posY < 0 ) {
		
		PORTE ^= (YELLOW_LED) | (RED_LED);
	}
	else {
		PORTE &= ~(YELLOW_LED);
		PORTE &= ~(RED_LED);
	}
}

ISR(PCINT0_vect) {

	collision_detected = 1;
}




int main(void) {

	uart_init(9600);

	Joystick_to_servo servo_struct;
	
	//DDRF &= ~(1 << PF2); //Push Button
	DDRF &= ~(1 << PF3); //Tilt Sensor

	DDRE |= (1 << PE3); //Yellow LED
	DDRE |= (1 << PE5); //Red LED 
	DDRB |= (1 << PB4); //Right Wheel
	DDRH |= (1 << PH6); //Left Wheel

	
	let_leds_blinking();
	collision_button();

	joystick_init();
	servo_init();

	collision_detected = 0;
	
	while(1) {

		//printf("%d Hello World!\n", counter++);
		//_delay_ms(1000);

		/*if (!BUTTON && !(TILT)) {
			
			PORTE |= RED_LED;
			PORTE |= YELLOW_LED;
			
			_delay_ms(50);
			
			PORTE &= ~RED_LED;
			PORTE &= ~YELLOW_LED;
			
			_delay_ms(50);
			
		} else if (BUTTON && !(TILT)) {
			
			PORTE |= RED_LED;

			_delay_ms(50);

			PORTE &= ~YELLOW_LED;

			_delay_ms(50);
			
		} else if (!BUTTON && TILT) {
			
			PORTE |= YELLOW_LED;

			_delay_ms(50);

			PORTE &= ~YELLOW_LED;

			_delay_ms(50);
			
		} else if (BUTTON && TILT) {
			
			PORTE &= ~YELLOW_LED;
			PORTE |= RED_LED;
			
			_delay_ms(500);
			
			PORTE &= ~RED_LED;
			PORTE |= YELLOW_LED;
			
			_delay_ms(500);
		}*/
	

		joystick_state = joystick_status();

		//printf("Joystick Digital Values: %d / %d\n", joystick_state.posX, joystick_state.posY);
		_delay_ms(20);
		
		servo_struct = calculate_joystick_to_servo(joystick_state);
		
		printf("Servo Values: %d / %d\n", servo_struct.w_1, servo_struct.w_2);
		
		if (collision_detected == 1) {

			servo_left(0);
			servo_right(0);
			
			if (!BUTTON) {
				collision_detected = 0;
			}
		} 
		else {

			servo_left(servo_struct.w_1);
			servo_right(servo_struct.w_2);
		}
	}

	return 0;
}





