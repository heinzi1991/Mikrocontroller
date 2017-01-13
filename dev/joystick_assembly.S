#include <avr/io.h>


.global joystick_init_asm
.global joystick_status_asm
.global adc_init_asm
.global adc_get_asm
.global calculate_values_asm


.section .text


joystick_init_asm:

call adc_init_asm
call realTimeCalibration


joystick_status_asm:

push r16
push r17

ldi r24, 0x00
call adc_get_asm
call get_digital_value_x
mov r16, r24


ldi r24, 0x01
call adc_get_asm
call get_digital_value_y
mov r17, r24

mov r24, r16
mov r25, r17

pop r17
pop r16

ret


adc_init_asm:

push r16
push r17

ldi r16, (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)
sts ADCSRA, r16

ldi r16, (0 << MUX5) | (0 << ADTS0) | (0 << ADTS1) | (0 << ADTS2)
sts ADCSRB, r16

ldi r16, (1 << REFS0)
sts ADMUX, r16

ldi r17, 0x03
com r17

mov r16, r17
lds r17, DDRF
and r17, r16
sts DDRF, r17

ldi r17, 0x03
com r17

mov r16, r17
lds r17, PORTF
and r17, r16
sts PORTF, r17

ldi r24, 0x00
call adc_get_asm

ldi r24, 0x01
call adc_get_asm

pop r17
pop r16

ret


adc_get_asm:

push r16
push r17

ldi r16, (1 << REFS0)
ldi r17, 0x07
and r17, r24
or r16, r17
sts ADMUX, r16

ldi r16, (1<<ADSC)
lds r17, ADCSRA
or r16, r17
sts ADCSRA, r16

while:
ldi r16, (1 << ADSC)
lds r17, ADCSRA
and r16, r17
cpi r16, 0
brne while

lds r24, ADCL
lds r25, ADCH

pop r17
pop r16

ret

calculate_values_asm:
ldi r18, 0xF4
ldi r19, 0x01
//cp r18, r24
//cpc r19, r25
sub r18, r24
sbc r19, r25
brmi small

small:
ldi r20, 100
ldi r21, 8
sub r21, r24
ldi r22, 0
sbc r22, r25




//if:
//ldi r18, 0xFE
//ldi r19, 0x01
//sub r18, r24
//sbc r19, r25
//brmi ret



