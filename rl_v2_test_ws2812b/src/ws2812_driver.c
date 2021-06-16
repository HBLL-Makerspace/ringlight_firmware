/*
 * ws2812_driver.c
 *
 * Created: 6/4/2021 4:51:09 PM
 *  Author: justb
 */ 

#include <ws2812_driver.h>
#include <clock_config.h>
#include <string.h>

static uint16_t WS2812_numLEDs;
static uint16_t WS2812_numBytes;
static uint8_t WS2812_pin_bp;
static uint8_t WS2812_brightness;
static uint8_t* WS2812_pixels;
static uint8_t WS2812_rOffset;
static uint8_t WS2812_gOffset;
static uint8_t WS2812_bOffset;
static uint8_t WS2812_wOffset;

volatile static uint8_t* WS2812_port;

void WS2812_init(ws2812Type t, uint16_t numLeds, uint8_t* port, uint8_t pin) {
	WS2812_set_type(t);
	WS2812_set_length(numLeds);
	WS2812_set_pin(port, pin);
}

void WS2812_show(void) {
	if ((!WS2812_pixels)) {
		return;
	}
	
	DISABLE_INTERRUPTS();
	
	volatile uint16_t i = WS2812_numBytes;
	volatile uint8_t *ptr = WS2812_pixels;
	volatile uint8_t b = *ptr++;
	volatile uint8_t hi;
	volatile uint8_t lo;
	
	// Hand-tuned assembly code issues data to the LED drivers at a specific
	// rate.  There's separate code for different CPU speeds (8, 12, 16 MHz)
	// for both the WS2811 (400 KHz) and WS2812 (800 KHz) drivers.  The
	// datastream timing for the LED drivers allows a little wiggle room each
	// way (listed in the datasheets), so the conditions for compiling each
	// case are set up for a range of frequencies rather than just the exact
	// 8, 12 or 16 MHz values, permitting use with some close-but-not-spot-on
	// devices (e.g. 16.5 MHz DigiSpark).  The ranges were arrived at based
	// on the datasheet figures and have not been extensively tested outside
	// the canonical 8/12/16 MHz speeds; there's no guarantee these will work
	// close to the extremes (or possibly they could be pushed further).
	// Keep in mind only one CPU speed case actually gets compiled; the
	// resulting program isn't as massive as it might look from source here.

	// 8 MHz(ish) AVRxt ---------------------------------------------------------
	#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)

	volatile uint8_t n1, n2 = 0;  // First, next bits out

	// We need to be able to write to the port register in one clock
	// to meet timing constraints here.

	// 10 instruction clocks per bit: HHxxxxxLLL
	// OUT instructions:              ^ ^    ^   (T=0,2,7)

	hi   = *WS2812_port |  (1 << WS2812_pin_bp);
	lo   = *WS2812_port & ~(1 << WS2812_pin_bp);
	n1 = lo;
	if(b & 0x80) n1 = hi;

	// Dirty trick: RJMPs proceeding to the next instruction are used
	// to delay two clock cycles in one instruction word (rather than
	// using two NOPs).  This was necessary in order to squeeze the
	// loop down to exactly 64 words -- the maximum possible for a
	// relative branch.

	asm volatile(
		"headD:"                   "\n\t" // Clk  Pseudocode
		// Bit 7:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"st   %a[port], %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 6"        "\n\t" // 1-2  if(b & 0x40)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 6:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"st   %a[port], %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 5"        "\n\t" // 1-2  if(b & 0x20)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 5:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"st   %a[port], %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 4"        "\n\t" // 1-2  if(b & 0x10)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 4:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"st   %a[port], %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 3"        "\n\t" // 1-2  if(b & 0x08)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 3:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"st   %a[port], %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 2"        "\n\t" // 1-2  if(b & 0x04)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 2:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"st   %a[port], %[n2]"    "\n\t" // 1    PORT = n2
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 1"        "\n\t" // 1-2  if(b & 0x02)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"rjmp .+0"                "\n\t" // 2    nop nop
		// Bit 1:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
		"st   %a[port], %[n1]"    "\n\t" // 1    PORT = n1
		"rjmp .+0"                "\n\t" // 2    nop nop
		"sbrc %[byte] , 0"        "\n\t" // 1-2  if(b & 0x01)
		"mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"sbiw %[count], 1"        "\n\t" // 2    i-- (don't act on Z flag yet)
		// Bit 0:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi
		"mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
		"st   %a[port], %[n2]"    "\n\t" // 1    PORT = n2
		"ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++
		"sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80)
		"mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo
		"brne headD"              "\n"   // 2    while(i) (Z flag set above)
	: [port]  "+e" (WS2812_port),
		[byte]  "+r" (b),
		[n1]    "+r" (n1),
		[n2]    "+r" (n2),
		[count] "+w" (i)
	: [ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));

	#elif (F_CPU >= 9500000UL) && (F_CPU <= 11100000UL)
	/*
	volatile uint8_t n1, n2 = 0;  // First, next bits out
	*/
	// 14 instruction clocks per bit: HHHHxxxxLLLLL
	// ST instructions:               ^   ^   ^   (T=0,4,7)
	volatile uint8_t next;

	hi   = *WS2812_port |  (1 << WS2812_pin_bp);
	lo   = *WS2812_port & ~(1 << WS2812_pin_bp);
	next = lo;
	if (b & 0x80) {
		next = hi;
	}

	// Don't "optimize" the OUT calls into the bitTime subroutine;
	// we're exploiting the RCALL and RET as 3- and 4-cycle NOPs!
	asm volatile(
		"headD:"                   "\n\t" //        (T =  0)
		"st   %a[port], %[hi]"    "\n\t" //        (T =  1)
		"rcall bitTimeD"          "\n\t" // Bit 7  (T = 14)
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 6
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 5
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 4
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 3
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 2
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 1
		// Bit 0:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi    (T =  1)
		"rjmp .+0"                "\n\t" // 2    nop nop      (T =  3)
		"ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++   (T =  5)
		"st   %a[port], %[next]"  "\n\t" // 1    PORT = next  (T =  6)
		"mov  %[next] , %[lo]"    "\n\t" // 1    next = lo    (T =  7)
		"sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80) (T =  8)
		"mov %[next] , %[hi]"    "\n\t" // 0-1    next = hi  (T =  9)
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo    (T = 10)
		"sbiw %[count], 1"        "\n\t" // 2    i--          (T = 12)
		"brne headD"              "\n\t" // 2    if(i != 0) -> (next byte)
		"rjmp doneD"             "\n\t"
		"bitTimeD:"               "\n\t" //      nop nop nop     (T =  4)
		"st   %a[port], %[next]" "\n\t" // 1    PORT = next     (T =  5)
		"mov  %[next], %[lo]"    "\n\t" // 1    next = lo       (T =  6)
		"rol  %[byte]"           "\n\t" // 1    b <<= 1         (T =  7)
		"sbrc %[byte], 7"        "\n\t" // 1-2  if(b & 0x80)    (T =  8)
		"mov %[next], %[hi]"    "\n\t" // 0-1   next = hi      (T =  9)
		"st   %a[port], %[lo]"   "\n\t" // 1    PORT = lo       (T = 10)
		"ret"                    "\n\t" // 4    nop nop nop nop (T = 14)
		"doneD:"                 "\n"
	: [port]  "+e" (WS2812_port),
		[byte]  "+r" (b),
		[next]  "+r" (next),
		[count] "+w" (i)
	: [ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));



	// 12 MHz(ish) AVRxt --------------------------------------------------------
	#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)

	// In the 12 MHz case, an optimized 800 KHz datastream (no dead time
	// between bytes) requires a PORT-specific loop similar to the 8 MHz
	// code (but a little more relaxed in this case).

	// 15 instruction clocks per bit: HHHHxxxxxxLLLLL
	// OUT instructions:              ^   ^     ^     (T=0,4,10)

	volatile uint8_t next;

	hi   = *WS2812_port |  (1 << WS2812_pin_bp);
	lo   = *WS2812_port & ~(1 << WS2812_pin_bp);
	next = lo;
	if (b & 0x80) {
		next = hi;
	}

		// Don't "optimize" the OUT calls into the bitTime subroutine;
		// we're exploiting the RCALL and RET as 3- and 4-cycle NOPs!
	asm volatile(
		"headD:"                   "\n\t" //        (T =  0)
		"st   %a[port], %[hi]"    "\n\t" //        (T =  1)
		"rcall bitTimeD"          "\n\t" // Bit 7  (T = 15)
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 6
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 5
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 4
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 3
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 2
		"st   %a[port], %[hi]"    "\n\t"
		"rcall bitTimeD"          "\n\t" // Bit 1
		// Bit 0:
		"st   %a[port], %[hi]"    "\n\t" // 1    PORT = hi    (T =  1)
		"rjmp .+0"                "\n\t" // 2    nop nop      (T =  3)
		"ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++   (T =  5)
		"st   %a[port], %[next]"  "\n\t" // 1    PORT = next  (T =  6)
		"mov  %[next] , %[lo]"    "\n\t" // 1    next = lo    (T =  7)
		"sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80) (T =  8)
		"mov %[next] , %[hi]"    "\n\t" // 0-1    next = hi  (T =  9)
		"nop"                     "\n\t" // 1                 (T = 10)
		"st   %a[port], %[lo]"    "\n\t" // 1    PORT = lo    (T = 11)
		"sbiw %[count], 1"        "\n\t" // 2    i--          (T = 13)
		"brne headD"              "\n\t" // 2    if(i != 0) -> (next byte)
		"rjmp doneD"             "\n\t"
		"bitTimeD:"               "\n\t" //      nop nop nop     (T =  4)
		"st   %a[port], %[next]" "\n\t" // 1    PORT = next     (T =  5)
		"mov  %[next], %[lo]"    "\n\t" // 1    next = lo       (T =  6)
		"rol  %[byte]"           "\n\t" // 1    b <<= 1         (T =  7)
		"sbrc %[byte], 7"        "\n\t" // 1-2  if(b & 0x80)    (T =  8)
		"mov %[next], %[hi]"    "\n\t" // 0-1   next = hi      (T =  9)
		"nop"                    "\n\t" // 1                    (T = 10)
		"st   %a[port], %[lo]"   "\n\t" // 1    PORT = lo       (T = 11)
		"ret"                    "\n\t" // 4    nop nop nop nop (T = 15)
		"doneD:"                 "\n"
	: [port]  "+e" (WS2812_port),
		[byte]  "+r" (b),
		[next]  "+r" (next),
		[count] "+w" (i)
	: [ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));


	// 16 MHz(ish) AVRxt ------------------------------------------------------
	#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)

	// WS2811 and WS2812 have different hi/lo duty cycles; this is
	// similar but NOT an exact copy of the prior 400-on-8 code.

	// 20 inst. clocks per bit: HHHHHxxxxxxxxLLLLLLL
	// ST instructions:         ^    ^       ^       (T=0,5,13)

	volatile uint8_t next, bit;

	hi   = *WS2812_port |  (1 << WS2812_pin_bp);
	lo   = *WS2812_port & ~(1 << WS2812_pin_bp);
	next = lo;
	bit  = 8;

	asm volatile(
		"head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
		"st   %a[port],  %[hi]"    "\n\t" // 1    PORT = hi     (T =  1)
		"nop"                      "\n\t" // 1    nop           (T =  2)
		"sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
		"mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
		"dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
		"st   %a[port],  %[next]"  "\n\t" // 1    PORT = next   (T =  6)
		"nop"                      "\n\t" // 1    nop           (T =  7)
		"mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
		"breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
		"rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
		"nop"                      "\n\t" // 1    nop           (T = 13)
		"st   %a[port],  %[lo]"    "\n\t" // 1    PORT = lo     (T = 14)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 16)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
		"rjmp head20"              "\n\t" // 2    -> head20 (next bit out) (T=20)
		"nextbyte20:"               "\n\t" //                    (T = 10)
		"ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
		"ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
		"st   %a[port], %[lo]"     "\n\t" // 1    PORT = lo     (T = 14)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 16)
		"sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
		"brne head20"             "\n"   // 2    if(i != 0) -> (next byte) (T=20)
	: [port]  "+e" (WS2812_port),
		[byte]  "+r" (b),
		[bit]   "+r" (bit),
		[next]  "+r" (next),
		[count] "+w" (i)
	: [ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));

	// 20 MHz(ish) AVRxt ------------------------------------------------------
	#elif (F_CPU >= 19000000UL) && (F_CPU <= 22000000L)


	// 25 inst. clocks per bit: HHHHHHHxxxxxxxxLLLLLLLLLL
	// ST instructions:         ^      ^       ^       (T=0,7,15)

	volatile uint8_t next, bit;

	hi   = *WS2812_port |  (1 << WS2812_pin_bp);
	lo   = *WS2812_port & ~(1 << WS2812_pin_bp);
	next = lo;
	bit  = 8;

	asm volatile(
		"head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
		"st   %a[port],  %[hi]"    "\n\t" // 1    PORT = hi     (T =  1)
		"sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
		"mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  3)
		"dec  %[bit]"              "\n\t" // 1    bit--         (T =  4)
		"nop"                      "\n\t" // 1    nop           (T =  5)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T =  7)
		"st   %a[port],  %[next]"  "\n\t" // 1    PORT = next   (T =  8)
		"mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  9)
		"breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
		"rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 11)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 13)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 15)
		"st   %a[port],  %[lo]"    "\n\t" // 1    PORT = lo     (T = 16)
		"nop"                      "\n\t" // 1    nop           (T = 17)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 19)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 21)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 23)
		"rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
		"nextbyte20:"               "\n\t" //                    (T = 11)
		"ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 12)
		"ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 14)
		"nop"                      "\n\t" // 1    nop           (T = 15)
		"st   %a[port], %[lo]"     "\n\t" // 1    PORT = lo     (T = 16)
		"nop"                      "\n\t" // 1    nop           (T = 17)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 19)
		"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 21)
		"sbiw %[count], 1"         "\n\t" // 2    i--           (T = 23)
		"brne head20"             "\n"   // 2    if(i != 0) -> (next byte)  ()
	: [port]  "+e" (WS2812_port),
		[byte]  "+r" (b),
		[bit]   "+r" (bit),
		[next]  "+r" (next),
		[count] "+w" (i)
	: [ptr]    "e" (ptr),
		[hi]     "r" (hi),
		[lo]     "r" (lo));

	#else
	#error "CPU SPEED NOT SUPPORTED"
	#endif
	
	ENABLE_INTERRUPTS();
}

void WS2812_set_pin(uint8_t* p, uint8_t bitn) {
	WS2812_port = p;
	WS2812_pin_bp = bitn;
}
	
void WS2812_set_pixel_color_RGB(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	if (n < WS2812_numLEDs) {
		if (WS2812_brightness) {
			r = (r * WS2812_brightness) >> 8;
			g = (g * WS2812_brightness) >> 8;
			b = (b * WS2812_brightness) >> 8;
		}
		
		uint8_t* p;
		if (WS2812_wOffset == WS2812_rOffset) {
			p = &WS2812_pixels[n * 3];
		} else {
			p = &WS2812_pixels[n * 4];
			p[WS2812_wOffset] = 0;
		}
		p[WS2812_rOffset] = r;
		p[WS2812_gOffset] = g;
		p[WS2812_bOffset] = b;
	}
}
	
void WS2812_set_pixel_color_RGBW(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	if (n < WS2812_numLEDs) {
		if (WS2812_brightness) {
			r = (r * WS2812_brightness) >> 8;
			g = (g * WS2812_brightness) >> 8;
			b = (b * WS2812_brightness) >> 8;
			w = (w * WS2812_brightness) >> 8;
		}
		
		uint8_t* p;
		if (WS2812_wOffset == WS2812_rOffset) {
			p = &WS2812_pixels[n * 3];
			} else {
			p = &WS2812_pixels[n * 4];
			p[WS2812_wOffset] = w;
		}
		p[WS2812_rOffset] = r;
		p[WS2812_gOffset] = g;
		p[WS2812_bOffset] = b;
	}
}
	
void WS2812_set_pixel_color_C(uint16_t n, uint32_t c) {
	if (n < WS2812_numLEDs) {
		uint8_t r = (uint8_t)(c >> 16);
		uint8_t g = (uint8_t)(c >> 8);
		uint8_t b = (uint8_t)c;
		
		if (WS2812_brightness) {
			r = (r * WS2812_brightness) >> 8;
			g = (g * WS2812_brightness) >> 8;
			b = (b * WS2812_brightness) >> 8;
		}
		
		uint8_t* p;
		if (WS2812_wOffset == WS2812_rOffset) {
			p = &WS2812_pixels[n * 3];
			} else {
			p = &WS2812_pixels[n * 4];
			uint8_t w = (uint8_t)(c >> 24);
			p[WS2812_wOffset] = (WS2812_brightness) ? ((w * WS2812_brightness) >> 8) : w;
		}
		p[WS2812_rOffset] = r;
		p[WS2812_gOffset] = g;
		p[WS2812_bOffset] = b;
	}
}
	
void WS2812_set_brightness(uint8_t b) {
	uint8_t newBrightness = b + 1;
	if (newBrightness != WS2812_brightness) { // Compare against prior value
		// Brightness has changed -- re-scale existing data in RAM
		uint8_t  c,
		*ptr           = WS2812_pixels,
		oldBrightness = WS2812_brightness - 1; // De-wrap old brightness value
		uint16_t scale;
		if (oldBrightness == 0) {
			scale = 0;  // Avoid /0
			} else if (b == 255) {
			scale = 65535 / oldBrightness;
			} else {
			scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
		}
		for (uint16_t i = 0; i < WS2812_numBytes; i++) {
			c      = *ptr;
			*ptr++ = (c * scale) >> 8;
		}
		WS2812_brightness = newBrightness;
	}
}
	
void WS2812_clear(void) {
	memset(WS2812_pixels, 0,  WS2812_numBytes);
}
	
void WS2812_set_length(uint16_t l) {
	if (WS2812_pixels) {
		free(WS2812_pixels);
	}
	
	WS2812_numBytes = l * ((WS2812_wOffset == WS2812_rOffset) ? 3 : 4);
	if ((WS2812_pixels = (uint8_t*) malloc(WS2812_numBytes))) {
		memset(WS2812_pixels, 0, WS2812_numBytes);
		WS2812_numLEDs = l;
	} else {
		WS2812_numLEDs = WS2812_numBytes = 0;
	}
}

void WS2812_set_type(ws2812Type t) {
	uint8_t oldThreeBytesPerPixel = (WS2812_wOffset == WS2812_rOffset); // false if RGBW
	
	WS2812_wOffset = (t >> 6) & 0b11;
	WS2812_rOffset = (t >> 4) & 0b11;
	WS2812_gOffset = (t >> 2) & 0b11;
	WS2812_bOffset = t & 0b11;
	
	if (WS2812_pixels) {
		uint8_t newThreeBytesPerPixel = (WS2812_wOffset == WS2812_rOffset);
		if (newThreeBytesPerPixel != oldThreeBytesPerPixel) {
			WS2812_set_length(WS2812_numLEDs);
		}
	}
}

uint8_t* WS2812_get_pixels(void) {
	return WS2812_pixels;
}
uint8_t WS2812_get_brightness(void) {
	return WS2812_brightness - 1;
}
uint8_t WS2812_get_pin(void) {
	return WS2812_pin_bp;
}
uint16_t WS2812_get_numpixels(void) {
	return WS2812_numLEDs;
}
uint32_t WS2812_get_pixel_color(uint16_t n) {
	if (n >= WS2812_numLEDs) {
		return 0;  // Out of bounds, return no color.
	}

	uint8_t *p;

	if (WS2812_wOffset == WS2812_rOffset) { // Is RGB-type device
		p = &WS2812_pixels[n * 3];
		if (WS2812_brightness) {
			// Stored color was decimated by setBrightness().  Returned value
			// attempts to scale back to an approximation of the original 24-bit
			// value used when setting the pixel color, but there will always be
			// some error -- those bits are simply gone.  Issue is most
			// pronounced at low brightness levels.
			return (((uint32_t)(p[WS2812_rOffset] << 8) / WS2812_brightness) << 16) |
			(((uint32_t)(p[WS2812_gOffset] << 8) / WS2812_brightness) <<  8) |
			( (uint32_t)(p[WS2812_bOffset] << 8) / WS2812_brightness       );
			} else {
			// No brightness adjustment has been made -- return 'raw' color
			return ((uint32_t)p[WS2812_rOffset] << 16) |
			((uint32_t)p[WS2812_gOffset] <<  8) |
			(uint32_t)p[WS2812_bOffset];
		}
		} else {                 // Is RGBW-type device
		p = &WS2812_pixels[n * 4];
		if (WS2812_brightness) { // Return scaled color
			return (((uint32_t)(p[WS2812_wOffset] << 8) / WS2812_brightness) << 24) |
			(((uint32_t)(p[WS2812_rOffset] << 8) / WS2812_brightness) << 16) |
			(((uint32_t)(p[WS2812_gOffset] << 8) / WS2812_brightness) <<  8) |
			( (uint32_t)(p[WS2812_bOffset] << 8) / WS2812_brightness       );
			} else { // Return raw color
			return ((uint32_t)p[WS2812_wOffset] << 24) |
			((uint32_t)p[WS2812_rOffset] << 16) |
			((uint32_t)p[WS2812_gOffset] <<  8) |
			(uint32_t)p[WS2812_bOffset];
		}
	}
}
