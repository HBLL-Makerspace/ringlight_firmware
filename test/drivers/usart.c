#include <drivers/usart.h>
#include <stdint.h>

#define RX_BUFFER_SIZE 8
#define TX_BUFFER_SZIE 1

uint8_t usart_rx_buffer[RX_BUFFER_SIZE];
uint8_t usart_tx_buffer[TX_BUFFER_SZIE];

uint8_t usart_rx_buffer_index;
uint8_t usart_tx_buffer_index;

/**
 * \brief Initialize the USART interface.
 *
 * \return \c void
 */
void USART_init(void) {
}

/**
 * \brief Enable the RX and TX in the USART interface.
 * Enables the clock to the USART, and enables the USART module by settings the TX
 * and RX enable bits in the control register
 *
 * \return void
 */
void USART_enable(void) {
}

/**
 * \brief Disables the USART interface.
 *
 * Disables the USART interface by clearing the RX and TX enable bits in the control register.
 * Then disables the clock to the USART interface.
 *
 * \return \c void
 */
void USART_disable(void) {
}

/**
 * \brief Get received data from the USART interface.
 *
 * \return Data from the data register in the USART interface
 */
uint8_t USART_get_data(void) {
  return 0;
}

/**
 * \brief Check if the USART can accept transmitt data.
 *
 * \return The status of the USART tx line.
 * \retval false The USART can not transmitt data
 * \retval true The USART can transmitt data
 */
bool USART_is_tx_ready(void) {
  return usart_tx_buffer_index == 0;
}

/**
 * \brief Check if the USART can receive data.
 *
 * \return The status of the USART rx line.
 * \retval false The USART can not receive data
 * \retval true The USART can receive data
 */
bool USART_is_rx_ready(void) {
  return usart_rx_buffer_index > 0;
}

/**
 * \brief Check if USART data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART_is_tx_busy(void) {
  return usart_tx_buffer_index;
}

/**
 * \brief Read one byte from the USART interface
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART interface
 */
uint8_t USART_read(void) {
  if (usart_rx_buffer_index > 0) {
    uint8_t val = usart_rx_buffer[usart_rx_buffer_index - 1];
    usart_rx_buffer_index--;
    return val;
  }
  return 0;
}

/**
 * \brief Write one byte to the USART interface
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return \c void
 */
void USART_write(const uint8_t data) {
  usart_tx_buffer[0] = data;
  usart_tx_buffer_index = 1;
}
