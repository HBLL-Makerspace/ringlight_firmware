#include <comm/comm_handler.h>
#include <drivers/usart.h>
#include <stdio.h>

extern uint8_t usart_rx_buffer[];
extern uint8_t usart_tx_buffer[];

extern uint8_t usart_rx_buffer_index;
extern uint8_t usart_tx_buffer_index;

extern uint8_t TxByteBuffer[];

int main(void) {
  printf("Running test: Testing send frame\n");

  uint8_t data[8] = {0xff, 0x05, 0x02, 0x01, 0x12, 0x34, 0x56, 0x00};
  uint8_t data_index = 0;

  printf("Incoming Data: [%x, %x, %x, %x, %x, %x, %x, %x]\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

  comm_handler_init();

  while (data_index < 8) {
    // Put stuff into the uart buffer
    // Only put one at a time for this test
    if (usart_rx_buffer_index == 0) {
      usart_rx_buffer[0] = data[data_index];
      usart_rx_buffer_index = 1;
      data_index++;
    }

    comm_handler_tick();

    printf("TX buffer: [%x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x]\n", TxByteBuffer[0], TxByteBuffer[1], TxByteBuffer[2], TxByteBuffer[3], TxByteBuffer[4], TxByteBuffer[5], TxByteBuffer[6], TxByteBuffer[7], TxByteBuffer[8],
           TxByteBuffer[9], TxByteBuffer[10], TxByteBuffer[11], TxByteBuffer[12], TxByteBuffer[13], TxByteBuffer[14], TxByteBuffer[15]);
  }

  for (uint8_t i = 0; i < 16; i++) {
    comm_handler_tick();
    printf("sending: %x\n", usart_tx_buffer[0]);
    usart_tx_buffer_index = 0;
  }
}