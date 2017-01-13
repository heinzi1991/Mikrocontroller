/**
 * @file uart.h
 *
 * @brief Low level driver specification for UART devices.
 *
 * @date $date$
 *
 * @author M.Lasagni
 *
 * $Id$
 */
#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

/**
 * @brief Initialize the UART module
 *
 * This function is called from the boot up code to initalize the UART module.
 *
 * @param baud_rate 	The baud rate of the connection.
 *
 */
void uart_init(uint32_t baud_rate);

///**
// * @brief Reads a byte from UART port.
// *
// * This function get a character from UART. The UART
// * module must be correctly initialized and configured for
// * this function to work.
// *
// * @param port The UART port to be used.
// * @param *c   Pointer to a char that receives the data.
// *
// * @return 0 if no char are present, 1 otherwise
// */
//uint8_t uart_read(uint8_t port, uint8_t *c);
//
///**
// * @brief Writes a byte on UART port.
// *
// * This function put a character on UART. The UART
// * module must be correctly initialized and configured for
// * this function to work.
// *
// * @param port The UART port to be used.
// * @param c    The char to write.
// */
//void uart_write(uint8_t port, uint8_t c);
//
///**
// * @brief Redirects stdout to a given UART port.
// *
// * Redirects the stdout channel to a given UART port.
// * Note that this alters the global variable stdout.
// * If you want to restore the previous behaviour, it
// * is your responsibility to backup to old value. The UART
// * module must be correctly initialized and configured for
// * the redirection to work.
// *
// * @param port The UART port to be used.
// *
// */
//void uart_redirect_stdout(uint8_t port);
//
///**
// * @brief Redirects stdin to a given UART port.
// *
// * Redirects the stdin channel to a given UART port.
// * Note that this alters the global variable stdin.
// * If you want to restore the previous behaviour, it
// * is your responsibility to backup to old value. The UART
// * module must be correctly initialized and configured for
// * the redirection to work.
// *
// * @param port The UART port to be used.
// *
// */
//void uart_redirect_stdin(uint8_t port);


#endif /* __UART_H__ */
