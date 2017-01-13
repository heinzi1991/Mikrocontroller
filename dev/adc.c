#include <avr/io.h>
#include "adc.h"

#if 1


/**
 * Initialize the ADC.
 *
 * Params: mask, defines which pins are enabled as ADC (1 is enabled, 0 disabled)
 *  -
 *
 * Return:
 *  -
 */
void adc_init(uint8_t mask) {
	uint8_t i;
	// 1. Enable the ADC by setting the bit ADEN in the register ADCSRA,
	//    and configure the prescaler with a  division factor of 128
	//    selecting ADPS2:0 as mentioned in the Atmega2560 datasheet.
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	ADCSRB = (0 << MUX5) | (0 << ADTS0) | (0 << ADTS1) | (0 << ADTS2);
	ADMUX = (1 << REFS0);

	DDRF &= ~(mask);
	PORTF &= ~(mask);

	// 3. Perform a dummy reading to clear the ADC before the first real
	//    reading, and ignore the result.
	for(i = 0; i < 8; i++) {
		if (mask & (1 << i)) {
			adc_get(i);
		}
	}
}

/**
 * Get a value from the ADC.
 *
 * Params: input, defines which pin to read: (e.g., set 3 for reading the third pin ADC2)
 *  -
 *
 * Return:
 *  - the raw value read on the ADC.
 */
uint16_t adc_get(uint8_t input) {

	ADMUX = (1 << REFS0) | (input & 0b00000111);

	// 1. Set the bit ADSC to start a conversion, but be careful not to
	//    alter other bits set before during the initialization!
	ADCSRA |= (1<<ADSC);

	// 2. Wait until the ADSC bit in the ADCRA register is cleared up again:
	//    that means the conversion is done.
	while(ADCSRA & (1<<ADSC));

	// 3. Return the value you can read in the register ADCH and ADCL.
	return ADC;
}
#endif
