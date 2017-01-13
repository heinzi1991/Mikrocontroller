/**
* @file uart.c
*
* @brief Low level driver for uart devices from AVR atmega family.
*
* @date 31/05/2012
*
* @author $Author: Matteo Lasagni$
*
* $Id$
*/

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"

#define RX_BUFF_SIZE	30

/*
 * Calculate the UART baud rate generator prescaler, based on the
 * global F_CPU setting, and the baud rate passed as parameter.  This
 * assumes the U2X bit will always be set.
 */
#define UBRR_VAL(rate)  ((2*F_CPU+rate*8UL)/(rate*16UL)-1UL)
#define UART_BAUD(rate) \
    (((F_CPU) + 4UL * (rate)) / (8UL * (rate)) - 1UL)

/* Disables the RX interrupt */
#define DISABLE_UART_RX_INT(p)  {	if (p == 1) (UCSR1B &= ~(1 << RXCIE1));\
									else (UCSR0B &= ~(1 << RXCIE0));\
								}

/* Enables the RX interrupt */
#define ENABLE_UART_RX_INT(p)   {	if (p == 1) (UCSR1B |= (1 << RXCIE1));\
									else (UCSR0B |= (1 << RXCIE0));\
								}

/* Disables the TX interrupt */
#define DISABLE_UART_TX_INT(p)  {	if (p == 1) (UCSR1B &= ~(1 << TXCIE1));\
									else (UCSR0B &= ~(1 << TXCIE0));\
								}

/* Enables the TX interrupt */
#define ENABLE_UART_TX_INT(p)   {	if (p == 1) (UCSR1B |= (1 << TXCIE1));\
									else (UCSR0B |= (1 << TXCIE0));\
								}

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

/* File descriptor for the STDIN and STDOUT*/
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

typedef struct {
	volatile uint8_t *UDR;
	volatile uint8_t *UCSRA;
	volatile uint8_t txwait;
	uint8_t rx_head;
	uint8_t volatile rx_size;
	uint8_t rx_buf[RX_BUFF_SIZE];
} uart_t;


#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega128__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega128RFA1__)
static uart_t uart_port[1] = {	{.UDR = &UDR0, .UCSRA = &UCSR0A, .txwait = 0, .rx_size = 0},	//UART0
								//{.UDR = &UDR1, .UCSRA = &UCSR1A, .txwait = 0, .rx_size = 0}		//UART1
};
#else
#    error "Please define the UART registers for your MCU!"
#endif

/*---------------------------------------------------------------------------*/
void uart_init(uint32_t baud_rate) {
	/* Calculate corresponding value for baud rateregister. */
	uint16_t baud_rate_reg = UBRR_VAL(baud_rate);

	cli();

	UBRR0H = (baud_rate_reg >> 8) & 0xFF;
	UBRR0L = (uint8_t)baud_rate_reg;

	UCSR0A = (1 << U2X0); 	//Faster async mode (UART clock divider = 8, instead of 16


	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	// Data Length is 8 bit
	// Receiver and transmitter are enabled. Receive and transmit interrrupts are enabled.
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << TXCIE0);

	uart_port[0].txwait = 0;

	sei();

	stdout = &uart_str;
	stdin = &uart_str;
}

static inline uint8_t uart_read(uint8_t *c) {
	uint8_t pos;
	uart_t *p = uart_port;

	if (p->rx_size > 0) {

		pos = p->rx_head;
		(*c) = p->rx_buf[pos];

		pos++;
		pos %= RX_BUFF_SIZE;

		DISABLE_UART_RX_INT(0);
		p->rx_head = pos;
		p->rx_size--;
		ENABLE_UART_RX_INT(0);
		return 1;
	}
	return 0;
}


int uart_putchar(char c, FILE *stream) {
	uart_t *p = uart_port;

	p->txwait = 1;
	*(p->UDR) = c;
	while(p->txwait);
	return 0;
}

int uart_getchar(FILE *stream) {
	uint8_t c;
	uart_t *p = uart_port;

	while(!uart_read(&c)) {
		if (*(p->UCSRA) & _BV(FE0)) {
			c = _FDEV_EOF;
			break;
		} else if (*(p->UCSRA) & _BV(DOR0)) {
			c = _FDEV_ERR;
			break;
		}
	}

	return c;
}

static inline void rx_buffer_write(uint8_t port) {
	uint8_t c, pos;
	uart_t *p = uart_port + port;

	c = *(p->UDR);

	if (p->rx_size < RX_BUFF_SIZE) {
		pos = p->rx_head + p->rx_size;
		pos %= RX_BUFF_SIZE;
		p->rx_buf[pos] = c;
		p->rx_size++;
	}
}

ISR(USART0_TX_vect) {
	uart_port[0].txwait = 0;
}

ISR(USART0_RX_vect) {
	rx_buffer_write(0);
}

//ISR(USART1_TX_vect) {
//	uart_port[1].txwait = 0;
//}
//
//ISR(USART1_RX_vect) {
//	rx_buffer_write(1);
//}
