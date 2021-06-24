#include <system_init.h>
#include <driver_init.h>
#include <ccp.h>
#include <port.h>
#include <atomic.h>

void system_init(void) {
    system_mcu_init();

	// This is the Configuration Change Protection register handling.
	// Write the io reg bits in the register to allow changes to some
	// protected registers. The changes must be written within 4
	// cpu cycles of writing to the CCP register.
	CCP = CCP_IOREG_gc;
	// Disable the prescaller. If not done the default clock speed is 3.3 MHz
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_6X_gc | 0 << CLKCTRL_PEN_bp; /* Prescaler disabled */

	INDICATOR_set_dir(PORT_DIR_OUT);
	FS1_set_dir(PORT_DIR_OUT);
	FS2_set_dir(PORT_DIR_OUT);
	H_EN_set_dir(PORT_DIR_OUT);
	V_EN_set_dir(PORT_DIR_OUT);
	N_EN_set_dir(PORT_DIR_OUT);
	LDAC_set_dir(PORT_DIR_OUT);

	INDICATOR_set_level(false);
	FS1_set_level(false);
	FS2_set_level(false);
	H_EN_set_level(false);
	V_EN_set_level(false);
	N_EN_set_level(false);
	LDAC_set_level(false);

    driver_init();

	ENABLE_INTERRUPTS();
}

void system_mcu_init(void) {
	/* On AVR devices all peripherals are enable from power on reset, this
	 * disables all peripherals to save power. Driver shall enable
	 * peripheral if used */

	/* Set all pins to low power mode */

	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
	}

	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
	}

	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
	}
}
