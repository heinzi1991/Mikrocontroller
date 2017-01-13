#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

void adc_init(uint8_t mask);
uint16_t adc_get(uint8_t input);



#endif /* __ADC_H__ */
