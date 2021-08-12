/**
 * \file
 */

#include<compiler.h>
#include<stdio.h>
#include<atomic.h>
#include<drivers/usart.h>

#if defined(__GNUC__)

int USART_printCHAR(char character, FILE *stream)
{
	USART_write(character);
	return 0;
}

FILE USART_stream = FDEV_SETUP_STREAM(USART_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
	USART_0_write(outChar);
	return outChar;
}
#endif

void USART_init(void) {
    USART0.BAUD = (uint16_t)USART_BAUD_RATE(9600);
    USART0.CTRLB = 0 << USART_MPCM_bp
                    | 0 << USART_ODME_bp
                    | 1 << USART_RXEN_bp
                    | USART_RXMODE_NORMAL_gc
                    | 0 << USART_SFDEN_bp
                    | 1 << USART_TXEN_bp;

    #if defined(__GNUC__)
	    stdout = &USART_stream;
    #endif
}

uint8_t USART_get_data(void) 
{
    return USART0.RXDATAL;
}

bool USART_is_tx_ready(void) 
{
    return (USART0.STATUS & USART_DREIF_bm);
}

bool USART_is_rx_ready(void)
{
	return (USART0.STATUS & USART_RXCIF_bm);
}

bool USART_is_tx_busy(void)
{
	return (!(USART0.STATUS & USART_TXCIF_bm));
}

uint8_t USART_read(void)
{
	while (!(USART0.STATUS & USART_RXCIF_bm))
		;
	return USART0.RXDATAL;
}

void USART_write(const uint8_t data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	USART0.TXDATAL = data;
}
