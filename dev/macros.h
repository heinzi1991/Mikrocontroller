#ifndef MACROS_H
#define MACROS_H

#include <avr/io.h>


//that are the macros of the input and output

#define TILT !(PINF & (1 << PF3))

#define BUTTON (PINB & (1 << PB5))

#define YELLOW_LED (1 << PE3)

#define RED_LED (1 << PE5)

#endif