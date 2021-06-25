/**
 * \file
 *
 * \brief Port related support
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

#ifndef PORT_INCLUDED
#define PORT_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>

/**
 * \brief Describes the pullup resistor states.
 */
enum port_pull_mode {
	PORT_PULL_OFF, /**< Disable the pullup resistor.*/
	PORT_PULL_UP, /**< Enable the pullup resistor.*/
};

/**
 * \brief Describes the port direction states.
 * 
 * There are 3 possible states to condifure every pin on the IC.
 * 1. INPUT
 * 2. OUTPUT
 * 3. FLOATING
 * 
 * This enum describes those states to the IC so that it is easier to 
 * see than using 0x0, 0x1 and 0x2
 */
enum port_dir {
	PORT_DIR_IN, /**< Set the port direction to be an input.*/
	PORT_DIR_OUT, /**< Set the port direction to be an output. */
	PORT_DIR_OFF, /**< Set the port direction to be floating. */
};

/**
 * \brief Set port pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull modes are defined by device used
 *
 * \param[in] pin       The pin number within port
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTA_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTA + 0x10 + pin);

	if (pull_mode == PORT_PULL_UP) {
		*port_pin_ctrl |= PORT_PULLUPEN_bm;
	} else if (pull_mode == PORT_PULL_OFF) {
		*port_pin_ctrl &= ~PORT_PULLUPEN_bm;
	}
}

/**
 * \brief Set port pin inverted mode
 *
 * Configure pin invert I/O or not
 *
 * \param[in] pin       The pin number within port
 * \param[in] inverted  Pin inverted mode
 */
static inline void PORTA_pin_set_inverted(const uint8_t pin, const bool inverted)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTA + 0x10 + pin);

	if (inverted) {
		*port_pin_ctrl |= PORT_INVEN_bm;
	} else {
		*port_pin_ctrl &= ~PORT_INVEN_bm;
	}
}

/**
 * \brief Set port pin input/sense configuration
 *
 * Enable/disable digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] pin pin number within port
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Interrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 *
 */
static inline void PORTA_pin_set_isc(const uint8_t pin, const PORT_ISC_t isc)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTA + 0x10 + pin);

	*port_pin_ctrl = (*port_pin_ctrl & ~PORT_ISC_gm) | isc;
}

/**
 * \brief Set port data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTA_set_port_dir(const uint8_t mask, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTA.DIR &= ~mask;
		break;
	case PORT_DIR_OUT:
		VPORTA.DIR |= mask;
		break;
	case PORT_DIR_OFF:
		/*/ should activate the pullup for power saving
		  but a bit costly to do it here */
		{
			for (uint8_t i = 0; i < 8; i++) {
				if (mask & 1 << i) {
					*((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
				}
			}
		}
		break;
	default:
		break;
	}
}

/**
 * \brief Set port pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within port
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTA_set_pin_dir(const uint8_t pin, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTA.DIR &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		VPORTA.DIR |= (1 << pin);
		break;
	case PORT_DIR_OFF:
		*((uint8_t *)&PORTA + 0x10 + pin) |= 1 << PORT_PULLUPEN_bp;
		break;
	default:
		break;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTA_set_port_level(const uint8_t mask, const bool level)
{
	if (level == true) {
		VPORTA.OUT |= mask;
	} else {
		VPORTA.OUT &= ~mask;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number within port
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTA_set_pin_level(const uint8_t pin, const bool level)
{
	if (level == true) {
		VPORTA.OUT |= (1 << pin);
	} else {
		VPORTA.OUT &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTA_toggle_port_level(const uint8_t mask)
{
	PORTA.OUTTGL = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] pin       The pin number within port
 */
static inline void PORTA_toggle_pin_level(const uint8_t pin)
{
	VPORTA.IN |= 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 */
static inline uint8_t PORTA_get_port_level()
{
	return VPORTA.IN;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on pins connected to a port
 */
static inline bool PORTA_get_pin_level(const uint8_t pin)
{
	return VPORTA.IN & (1 << pin);
}

/**
 * \brief Write value to Port
 *
 * Write directly to the port OUT register
 *
 * \param[in] value Value to write to the port register
 */
static inline void PORTA_write_port(const uint8_t value)
{
	VPORTA.OUT = value;
}

/**
 * \brief Set port pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull modes are defined by device used
 *
 * \param[in] pin       The pin number within port
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTB_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTB + 0x10 + pin);

	if (pull_mode == PORT_PULL_UP) {
		*port_pin_ctrl |= PORT_PULLUPEN_bm;
	} else if (pull_mode == PORT_PULL_OFF) {
		*port_pin_ctrl &= ~PORT_PULLUPEN_bm;
	}
}

/**
 * \brief Set port pin inverted mode
 *
 * Configure pin invert I/O or not
 *
 * \param[in] pin       The pin number within port
 * \param[in] inverted  Pin inverted mode
 */
static inline void PORTB_pin_set_inverted(const uint8_t pin, const bool inverted)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTB + 0x10 + pin);

	if (inverted) {
		*port_pin_ctrl |= PORT_INVEN_bm;
	} else {
		*port_pin_ctrl &= ~PORT_INVEN_bm;
	}
}

/**
 * \brief Set port pin input/sense configuration
 *
 * Enable/disable digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] pin pin number within port
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Interrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 *
 */
static inline void PORTB_pin_set_isc(const uint8_t pin, const PORT_ISC_t isc)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTB + 0x10 + pin);

	*port_pin_ctrl = (*port_pin_ctrl & ~PORT_ISC_gm) | isc;
}

/**
 * \brief Set port data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTB_set_port_dir(const uint8_t mask, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTB.DIR &= ~mask;
		break;
	case PORT_DIR_OUT:
		VPORTB.DIR |= mask;
		break;
	case PORT_DIR_OFF:
		/*/ should activate the pullup for power saving
		  but a bit costly to do it here */
		{
			for (uint8_t i = 0; i < 8; i++) {
				if (mask & 1 << i) {
					*((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
				}
			}
		}
		break;
	default:
		break;
	}
}

/**
 * \brief Set port pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within port
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTB_set_pin_dir(const uint8_t pin, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTB.DIR &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		VPORTB.DIR |= (1 << pin);
		break;
	case PORT_DIR_OFF:
		*((uint8_t *)&PORTB + 0x10 + pin) |= 1 << PORT_PULLUPEN_bp;
		break;
	default:
		break;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTB_set_port_level(const uint8_t mask, const bool level)
{
	if (level == true) {
		VPORTB.OUT |= mask;
	} else {
		VPORTB.OUT &= ~mask;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number within port
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTB_set_pin_level(const uint8_t pin, const bool level)
{
	if (level == true) {
		VPORTB.OUT |= (1 << pin);
	} else {
		VPORTB.OUT &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTB_toggle_port_level(const uint8_t mask)
{
	PORTB.OUTTGL = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] pin       The pin number within port
 */
static inline void PORTB_toggle_pin_level(const uint8_t pin)
{
	VPORTB.IN |= 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 */
static inline uint8_t PORTB_get_port_level()
{
	return VPORTB.IN;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on pins connected to a port
 */
static inline bool PORTB_get_pin_level(const uint8_t pin)
{
	return VPORTB.IN & (1 << pin);
}

/**
 * \brief Write value to Port
 *
 * Write directly to the port OUT register
 *
 * \param[in] value Value to write to the port register
 */
static inline void PORTB_write_port(const uint8_t value)
{
	VPORTB.OUT = value;
}

/**
 * \brief Set port pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull modes are defined by device used
 *
 * \param[in] pin       The pin number within port
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTC_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTC + 0x10 + pin);

	if (pull_mode == PORT_PULL_UP) {
		*port_pin_ctrl |= PORT_PULLUPEN_bm;
	} else if (pull_mode == PORT_PULL_OFF) {
		*port_pin_ctrl &= ~PORT_PULLUPEN_bm;
	}
}

/**
 * \brief Set port pin inverted mode
 *
 * Configure pin invert I/O or not
 *
 * \param[in] pin       The pin number within port
 * \param[in] inverted  Pin inverted mode
 */
static inline void PORTC_pin_set_inverted(const uint8_t pin, const bool inverted)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTC + 0x10 + pin);

	if (inverted) {
		*port_pin_ctrl |= PORT_INVEN_bm;
	} else {
		*port_pin_ctrl &= ~PORT_INVEN_bm;
	}
}

/**
 * \brief Set port pin input/sense configuration
 *
 * Enable/disable digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] pin pin number within port
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Interrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 *
 */
static inline void PORTC_pin_set_isc(const uint8_t pin, const PORT_ISC_t isc)
{
	volatile uint8_t *port_pin_ctrl = ((uint8_t *)&PORTC + 0x10 + pin);

	*port_pin_ctrl = (*port_pin_ctrl & ~PORT_ISC_gm) | isc;
}

/**
 * \brief Set port data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTC_set_port_dir(const uint8_t mask, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTC.DIR &= ~mask;
		break;
	case PORT_DIR_OUT:
		VPORTC.DIR |= mask;
		break;
	case PORT_DIR_OFF:
		/*/ should activate the pullup for power saving
		  but a bit costly to do it here */
		{
			for (uint8_t i = 0; i < 8; i++) {
				if (mask & 1 << i) {
					*((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
				}
			}
		}
		break;
	default:
		break;
	}
}

/**
 * \brief Set port pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within port
 * \param[in] dir       PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTC_set_pin_dir(const uint8_t pin, const enum port_dir dir)
{
	switch (dir) {
	case PORT_DIR_IN:
		VPORTC.DIR &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		VPORTC.DIR |= (1 << pin);
		break;
	case PORT_DIR_OFF:
		*((uint8_t *)&PORTC + 0x10 + pin) |= 1 << PORT_PULLUPEN_bp;
		break;
	default:
		break;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTC_set_port_level(const uint8_t mask, const bool level)
{
	if (level == true) {
		VPORTC.OUT |= mask;
	} else {
		VPORTC.OUT &= ~mask;
	}
}

/**
 * \brief Set port level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number within port
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTC_set_pin_level(const uint8_t pin, const bool level)
{
	if (level == true) {
		VPORTC.OUT |= (1 << pin);
	} else {
		VPORTC.OUT &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTC_toggle_port_level(const uint8_t mask)
{
	PORTC.OUTTGL = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] pin       The pin number within port
 */
static inline void PORTC_toggle_pin_level(const uint8_t pin)
{
	VPORTC.IN |= 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 */
static inline uint8_t PORTC_get_port_level()
{
	return VPORTC.IN;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on pins connected to a port
 */
static inline bool PORTC_get_pin_level(const uint8_t pin)
{
	return VPORTC.IN & (1 << pin);
}

/**
 * \brief Write value to Port
 *
 * Write directly to the port OUT register
 *
 * \param[in] value Value to write to the port register
 */
static inline void PORTC_write_port(const uint8_t value)
{
	VPORTC.OUT = value;
}

/**
 * \brief Set INDICATOR pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void INDICATOR_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set INDICATOR data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void INDICATOR_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(1, dir);
}

/**
 * \brief Set INDICATOR input/sense configuration
 *
 * Enable/disable INDICATOR digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void INDICATOR_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(1, isc);
}

/**
 * \brief Set INDICATOR inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on INDICATOR is inverted
 *                     false = I/O on INDICATOR is not inverted
 */
static inline void INDICATOR_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(1, inverted);
}

/**
 * \brief Set INDICATOR level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void INDICATOR_set_level(const bool level)
{
	PORTA_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on INDICATOR
 *
 * Toggle the pin level
 */
static inline void INDICATOR_toggle_level()
{
	PORTA_toggle_pin_level(1);
}

/**
 * \brief Get level on INDICATOR
 *
 * Reads the level on a pin
 */
static inline bool INDICATOR_get_level()
{
	return PORTA_get_pin_level(1);
}

/**
 * \brief Set CLED_CTRL pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void CLED_CTRL_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(2, pull_mode);
}

/**
 * \brief Set CLED_CTRL data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void CLED_CTRL_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(2, dir);
}

/**
 * \brief Set CLED_CTRL input/sense configuration
 *
 * Enable/disable CLED_CTRL digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void CLED_CTRL_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(2, isc);
}

/**
 * \brief Set CLED_CTRL inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on CLED_CTRL is inverted
 *                     false = I/O on CLED_CTRL is not inverted
 */
static inline void CLED_CTRL_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(2, inverted);
}

/**
 * \brief Set CLED_CTRL level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void CLED_CTRL_set_level(const bool level)
{
	PORTA_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on CLED_CTRL
 *
 * Toggle the pin level
 */
static inline void CLED_CTRL_toggle_level()
{
	PORTA_toggle_pin_level(2);
}

/**
 * \brief Get level on CLED_CTRL
 *
 * Reads the level on a pin
 */
static inline bool CLED_CTRL_get_level()
{
	return PORTA_get_pin_level(2);
}

/**
 * \brief Set H_EN pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void H_EN_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(3, pull_mode);
}

/**
 * \brief Set H_EN data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void H_EN_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(3, dir);
}

/**
 * \brief Set H_EN input/sense configuration
 *
 * Enable/disable H_EN digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void H_EN_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(3, isc);
}

/**
 * \brief Set H_EN inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on H_EN is inverted
 *                     false = I/O on H_EN is not inverted
 */
static inline void H_EN_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(3, inverted);
}

/**
 * \brief Set H_EN level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void H_EN_set_level(const bool level)
{
	PORTA_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on H_EN
 *
 * Toggle the pin level
 */
static inline void H_EN_toggle_level()
{
	PORTA_toggle_pin_level(3);
}

/**
 * \brief Get level on H_EN
 *
 * Reads the level on a pin
 */
static inline bool H_EN_get_level()
{
	return PORTA_get_pin_level(3);
}

/**
 * \brief Set V_EN pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void V_EN_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(4, pull_mode);
}

/**
 * \brief Set V_EN data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void V_EN_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(4, dir);
}

/**
 * \brief Set V_EN input/sense configuration
 *
 * Enable/disable V_EN digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void V_EN_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(4, isc);
}

/**
 * \brief Set V_EN inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on V_EN is inverted
 *                     false = I/O on V_EN is not inverted
 */
static inline void V_EN_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(4, inverted);
}

/**
 * \brief Set V_EN level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void V_EN_set_level(const bool level)
{
	PORTA_set_pin_level(4, level);
}

/**
 * \brief Toggle output level on V_EN
 *
 * Toggle the pin level
 */
static inline void V_EN_toggle_level()
{
	PORTA_toggle_pin_level(4);
}

/**
 * \brief Get level on V_EN
 *
 * Reads the level on a pin
 */
static inline bool V_EN_get_level()
{
	return PORTA_get_pin_level(4);
}

/**
 * \brief Set N_EN pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void N_EN_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(5, pull_mode);
}

/**
 * \brief Set N_EN data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void N_EN_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(5, dir);
}

/**
 * \brief Set N_EN input/sense configuration
 *
 * Enable/disable N_EN digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void N_EN_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(5, isc);
}

/**
 * \brief Set N_EN inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on N_EN is inverted
 *                     false = I/O on N_EN is not inverted
 */
static inline void N_EN_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(5, inverted);
}

/**
 * \brief Set N_EN level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void N_EN_set_level(const bool level)
{
	PORTA_set_pin_level(5, level);
}

/**
 * \brief Toggle output level on N_EN
 *
 * Toggle the pin level
 */
static inline void N_EN_toggle_level()
{
	PORTA_toggle_pin_level(5);
}

/**
 * \brief Get level on N_EN
 *
 * Reads the level on a pin
 */
static inline bool N_EN_get_level()
{
	return PORTA_get_pin_level(5);
}

/**
 * \brief Set FS1 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void FS1_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(6, pull_mode);
}

/**
 * \brief Set FS1 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void FS1_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(6, dir);
}

/**
 * \brief Set FS1 input/sense configuration
 *
 * Enable/disable FS1 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void FS1_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(6, isc);
}

/**
 * \brief Set FS1 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on FS1 is inverted
 *                     false = I/O on FS1 is not inverted
 */
static inline void FS1_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(6, inverted);
}

/**
 * \brief Set FS1 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void FS1_set_level(const bool level)
{
	PORTA_set_pin_level(6, level);
}

/**
 * \brief Toggle output level on FS1
 *
 * Toggle the pin level
 */
static inline void FS1_toggle_level()
{
	PORTA_toggle_pin_level(6);
}

/**
 * \brief Get level on FS1
 *
 * Reads the level on a pin
 */
static inline bool FS1_get_level()
{
	return PORTA_get_pin_level(6);
}

/**
 * \brief Set FS2 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void FS2_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(7, pull_mode);
}

/**
 * \brief Set FS2 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void FS2_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(7, dir);
}

/**
 * \brief Set FS2 input/sense configuration
 *
 * Enable/disable FS2 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void FS2_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(7, isc);
}

/**
 * \brief Set FS2 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on FS2 is inverted
 *                     false = I/O on FS2 is not inverted
 */
static inline void FS2_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(7, inverted);
}

/**
 * \brief Set FS2 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void FS2_set_level(const bool level)
{
	PORTA_set_pin_level(7, level);
}

/**
 * \brief Toggle output level on FS2
 *
 * Toggle the pin level
 */
static inline void FS2_toggle_level()
{
	PORTA_toggle_pin_level(7);
}

/**
 * \brief Get level on FS2
 *
 * Reads the level on a pin
 */
static inline bool FS2_get_level()
{
	return PORTA_get_pin_level(7);
}

/**
 * \brief Set SCL pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void SCL_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set SCL data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void SCL_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(0, dir);
}

/**
 * \brief Set SCL input/sense configuration
 *
 * Enable/disable SCL digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void SCL_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(0, isc);
}

/**
 * \brief Set SCL inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on SCL is inverted
 *                     false = I/O on SCL is not inverted
 */
static inline void SCL_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(0, inverted);
}

/**
 * \brief Set SCL level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void SCL_set_level(const bool level)
{
	PORTB_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on SCL
 *
 * Toggle the pin level
 */
static inline void SCL_toggle_level()
{
	PORTB_toggle_pin_level(0);
}

/**
 * \brief Get level on SCL
 *
 * Reads the level on a pin
 */
static inline bool SCL_get_level()
{
	return PORTB_get_pin_level(0);
}

/**
 * \brief Set SDA pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void SDA_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set SDA data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void SDA_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(1, dir);
}

/**
 * \brief Set SDA input/sense configuration
 *
 * Enable/disable SDA digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void SDA_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(1, isc);
}

/**
 * \brief Set SDA inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on SDA is inverted
 *                     false = I/O on SDA is not inverted
 */
static inline void SDA_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(1, inverted);
}

/**
 * \brief Set SDA level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void SDA_set_level(const bool level)
{
	PORTB_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on SDA
 *
 * Toggle the pin level
 */
static inline void SDA_toggle_level()
{
	PORTB_toggle_pin_level(1);
}

/**
 * \brief Get level on SDA
 *
 * Reads the level on a pin
 */
static inline bool SDA_get_level()
{
	return PORTB_get_pin_level(1);
}

/**
 * \brief Set TX pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void TX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(2, pull_mode);
}

/**
 * \brief Set TX data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void TX_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(2, dir);
}

/**
 * \brief Set TX input/sense configuration
 *
 * Enable/disable TX digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void TX_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(2, isc);
}

/**
 * \brief Set TX inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on TX is inverted
 *                     false = I/O on TX is not inverted
 */
static inline void TX_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(2, inverted);
}

/**
 * \brief Set TX level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void TX_set_level(const bool level)
{
	PORTB_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on TX
 *
 * Toggle the pin level
 */
static inline void TX_toggle_level()
{
	PORTB_toggle_pin_level(2);
}

/**
 * \brief Get level on TX
 *
 * Reads the level on a pin
 */
static inline bool TX_get_level()
{
	return PORTB_get_pin_level(2);
}

/**
 * \brief Set RX pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void RX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(3, pull_mode);
}

/**
 * \brief Set RX data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void RX_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(3, dir);
}

/**
 * \brief Set RX input/sense configuration
 *
 * Enable/disable RX digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void RX_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(3, isc);
}

/**
 * \brief Set RX inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on RX is inverted
 *                     false = I/O on RX is not inverted
 */
static inline void RX_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(3, inverted);
}

/**
 * \brief Set RX level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void RX_set_level(const bool level)
{
	PORTB_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on RX
 *
 * Toggle the pin level
 */
static inline void RX_toggle_level()
{
	PORTB_toggle_pin_level(3);
}

/**
 * \brief Get level on RX
 *
 * Reads the level on a pin
 */
static inline bool RX_get_level()
{
	return PORTB_get_pin_level(3);
}

/**
 * \brief Set LDAC pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void LDAC_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(4, pull_mode);
}

/**
 * \brief Set LDAC data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void LDAC_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(4, dir);
}

/**
 * \brief Set LDAC input/sense configuration
 *
 * Enable/disable LDAC digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LDAC_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(4, isc);
}

/**
 * \brief Set LDAC inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on LDAC is inverted
 *                     false = I/O on LDAC is not inverted
 */
static inline void LDAC_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(4, inverted);
}

/**
 * \brief Set LDAC level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void LDAC_set_level(const bool level)
{
	PORTB_set_pin_level(4, level);
}

/**
 * \brief Toggle output level on LDAC
 *
 * Toggle the pin level
 */
static inline void LDAC_toggle_level()
{
	PORTB_toggle_pin_level(4);
}

/**
 * \brief Get level on LDAC
 *
 * Reads the level on a pin
 */
static inline bool LDAC_get_level()
{
	return PORTB_get_pin_level(4);
}

/**
 * \brief Set BSY pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void BSY_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(5, pull_mode);
}

/**
 * \brief Set BSY data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void BSY_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(5, dir);
}

/**
 * \brief Set BSY input/sense configuration
 *
 * Enable/disable BSY digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void BSY_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(5, isc);
}

/**
 * \brief Set BSY inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on BSY is inverted
 *                     false = I/O on BSY is not inverted
 */
static inline void BSY_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(5, inverted);
}

/**
 * \brief Set BSY level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void BSY_set_level(const bool level)
{
	PORTB_set_pin_level(5, level);
}

/**
 * \brief Toggle output level on BSY
 *
 * Toggle the pin level
 */
static inline void BSY_toggle_level()
{
	PORTB_toggle_pin_level(5);
}

/**
 * \brief Get level on BSY
 *
 * Reads the level on a pin
 */
static inline bool BSY_get_level()
{
	return PORTB_get_pin_level(5);
}

/**
 * \brief Set LOAD pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void LOAD_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(3, pull_mode);
}

/**
 * \brief Set LOAD data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void LOAD_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(3, dir);
}

/**
 * \brief Set LOAD input/sense configuration
 *
 * Enable/disable LOAD digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LOAD_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(3, isc);
}

/**
 * \brief Set LOAD inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on LOAD is inverted
 *                     false = I/O on LOAD is not inverted
 */
static inline void LOAD_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(3, inverted);
}

/**
 * \brief Set LOAD level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void LOAD_set_level(const bool level)
{
	PORTC_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on LOAD
 *
 * Toggle the pin level
 */
static inline void LOAD_toggle_level()
{
	PORTC_toggle_pin_level(3);
}

/**
 * \brief Get level on LOAD
 *
 * Reads the level on a pin
 */
static inline bool LOAD_get_level()
{
	return PORTC_get_pin_level(3);
}

#ifdef __cplusplus
}
#endif

#endif /* PORT_INCLUDED */
