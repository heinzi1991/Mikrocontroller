#include "servo.h"
#include "stdlib.h"
#include "stdio.h"
#include "macros.h"

#define LEFT 23
#define RIGHT 23

void servo_init() {
	
	cli();
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	sei();
	
	OCR2A = RIGHT;
	OCR2B = LEFT;
}


void servo_left(int velocity) {
	
	uint8_t wert;
	
	if (velocity == 0) {
		
		wert = LEFT;
	}
	else {
		
		wert = (((-velocity + 100) * 16) / 200) + 16;
	}
	
	uint8_t old = OCR2B;
	
	if (old < wert) {
		
		while(old < wert) {
			
			old++;
			OCR2B = old;
			_delay_ms(25);
		}
	}
	else {
		while(old > wert) {
			old--;
			OCR2B = old;
			_delay_ms(25);
		}
	}
}

void servo_right(int velocity) {
	
	uint8_t wert;
	
	if (velocity == 0) {
		
		wert = RIGHT;
	}
	else {
		
		wert = (((velocity + 100) * 16) / 200) + 16;
	}
	
	uint8_t old = OCR2A;
	
	if (old < wert) {
		
		while(old < wert) {
			
			old++;
			OCR2A = old;
			_delay_ms(25);
		}
	}
	
	else {
		while(old > wert) {
			old--;
			OCR2A = old;
			_delay_ms(25);
		}
	}	
}
