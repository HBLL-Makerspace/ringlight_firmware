/**
 * \file
 * \author Ben Brenkman
 * \date 29 June 2021
 * 
 * Basic USART dirver.
 */

#ifndef USART_H__
#define USART_H__

#include<stdbool.h>
#include<compiler.h>
#include<clock_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

/**
 * \brief Initialize the USART interface.
 * 
 * \return \c void
 */
void USART_init(void);

/** 
 * \brief Enable the RX and TX in the USART interface.
 * Enables the clock to the USART, and enables the USART module by settings the TX
 * and RX enable bits in the control register
 * 
 * \return void
 */
void USART_enable(void);

/**
 * \brief Disables the USART interface.
 * 
 * Disables the USART interface by clearing the RX and TX enable bits in the control register.
 * Then disables the clock to the USART interface.
 * 
 * \return \c void
 */
void USART_disable(void);

/**
 * \brief Get received data from the USART interface.
 * 
 * \return Data from the data register in the USART interface
 */
uint8_t USART_get_data(void);

/**
 * \brief Check if the USART can accept transmitt data.
 * 
 * \return The status of the USART tx line.
 * \retval false The USART can not transmitt data
 * \retval true The USART can transmitt data
 */
bool USART_is_tx_ready(void);

/**
 * \brief Check if the USART can receive data.
 * 
 * \return The status of the USART rx line.
 * \retval false The USART can not receive data
 * \retval true The USART can receive data
 */
bool USART_is_rx_ready(void);

/**
 * \brief Check if USART data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART_is_tx_busy(void);

/**
 * \brief Read one byte from the USART interface
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART interface
 */
uint8_t USART_read(void);

/**
 * \brief Write one byte to the USART interface
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return \c void
 */
void USART_write(const uint8_t data);

#ifdef __cplusplus
}
#endif

#endif