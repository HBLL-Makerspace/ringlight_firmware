/*
 * ws2812_driver.h
 *
 * Created: 6/4/2021 4:40:15 PM
 *  Author: justb
 * 
 * Modified code from the tinyNeoPixel library developed for the megaTinyCore
 * Modified to work as a single instance library, can only have a single strip
 * Works without classes.
 * 
 */ 


#ifndef WS2812_DRIVER_H_
#define WS2812_DRIVER_H_

#include <compiler.h>
#include <atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

// RGB WS2812 permutations; white and red offsets are always same
// Offset:         W          R          G          B
#define WS2812_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2))
#define WS2812_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1))
#define WS2812_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2))
#define WS2812_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1))
#define WS2812_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0))
#define WS2812_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0))

// RGBW WS2812 permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define WS2812_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3))
#define WS2812_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2))
#define WS2812_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3))
#define WS2812_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2))
#define WS2812_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1))
#define WS2812_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1))

#define WS2812_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3))
#define WS2812_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2))
#define WS2812_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3))
#define WS2812_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2))
#define WS2812_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1))
#define WS2812_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1))

#define WS2812_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3))
#define WS2812_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2))
#define WS2812_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3))
#define WS2812_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2))
#define WS2812_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1))
#define WS2812_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1))

#define WS2812_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0))
#define WS2812_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0))
#define WS2812_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0))
#define WS2812_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0))
#define WS2812_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0))
#define WS2812_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0))

#define WS2812_KHZ800 0x0000 ///< 800 KHz data transmission

/* A pre-calculated 8-bit sine look-up table stored in flash for use
with the sine8() function. This is apparently of use in some animation
algorithms. If __AVR_ARCH__==103, then all of the flash is memory
mapped, and we can simply declare it const, access it like a
normal variable, and it won't be copied to RAM.
AVRxt devices with too much flash for all of it to be mapped
which includes the AVR64Dx and AVR128Dx parts. DxCore defines a
.section for the area of PROGMEM that is mapped by default, and
a MAPPED_PROGMEM macro. A variable declared const MAPPED_PROGMEM can
be accessed normally, but will be stored in the flash and not copied to RAM.
Finally, if neither of those are an option - it gets declared with PROGMEM
   Copy & paste this snippet into a Python REPL to regenerate:
import math
for x in range(256):
    print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
    if x&15 == 15: print
*/
#if (__AVR_ARCH__==103)
  // All out flash is mapped - yay!
  static const uint8_t _NeoPixelSineTable[256] = {
#elif defined(MAPPED_PROGMEM)
  // Some of it is - but we can put stuff there - yay!
  static const uint8_t MAPPED_PROGMEM _NeoPixelSineTable[256] = {
#else
  // Back to progmem...
  static const uint8_t PROGMEM _NeoPixelSineTable[256] = {
#endif
  128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,
  176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,
  218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,
  245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,
  255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,
  245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,
  218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,
  176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,
  128,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,
   79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,
   37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,
   10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
   10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
   37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
   79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124};

/* Similar to above, but for an 8-bit gamma-correction table.
   Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
    print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
    if x&15 == 15: print
*/
#if (__AVR_ARCH__==103)
  // All our flash is mapped - yay!
  static const uint8_t _NeoPixelGammaTable[256] = {
#elif defined(MAPPED_PROGMEM)
  // Some of it is - but we can put stuff there - yay!
  static const uint8_t MAPPED_PROGMEM _NeoPixelGammaTable[256] = {
#else
  // Back to progmem...
  static const uint8_t PROGMEM _NeoPixelGammaTable[256] = {
#endif
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};
  
  static uint8_t    WS2812_gamma8(uint8_t x) {
	  #if (__AVR_ARCH__==103 || defined(MAPPED_PROGMEM))
	  return _NeoPixelGammaTable[x];
	  #else
	  return pgm_read_byte(&_NeoPixelGammaTable[x]);
	  #endif
  }
  
  //static uint8_t    WS2812_sine8(uint8_t x) { // 0-255 in, 0-255 out
	  //#if (__AVR_ARCH__==103 || defined(MAPPED_PROGMEM))
	  //return _NeoPixelSineTable[x];
	  //#else     // We had to put it in PROGMEM, and that's how we get it out
	  //return pgm_read_byte(&_NeoPixelSineTable[x]); // 0-255 in, 0-255 out
	  //#endif
  //}
  
  typedef uint8_t ws2812Type;
  
  void WS2812_init(ws2812Type t, uint16_t numLeds, uint8_t* port, uint8_t pin);
  void WS2812_show(void);
  void WS2812_set_pin(uint8_t* p, uint8_t bitn);
  void WS2812_set_pixel_color_RGB(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void WS2812_set_pixel_color_RGBW(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  void WS2812_set_pixel_color_C(uint16_t n, uint32_t c);
  void WS2812_set_brightness(uint8_t b);
  void WS2812_clear(void);
  void WS2812_set_length(uint16_t l);
  void WS2812_set_type(ws2812Type t);
  
  uint8_t* WS2812_get_pixels(void);
  uint8_t WS2812_get_brightness(void);
  uint8_t WS2812_get_pin(void);
  uint16_t WS2812_get_numpixels(void);
  uint32_t WS2812_get_pixel_color(uint16_t p);
  
  /*!
    @brief   Convert separate red, green and blue values into a single
             "packed" 32-bit RGB color.
    @param   r  Red brightness, 0 to 255.
    @param   g  Green brightness, 0 to 255.
    @param   b  Blue brightness, 0 to 255.
    @return  32-bit packed RGB value, which can then be assigned to a
             variable for later use or passed to the setPixelColor()
             function. Packed RGB format is predictable, regardless of
             LED strand color order.
  */
  static uint32_t   WS2812_Color_RGB(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
  /*!
    @brief   Convert separate red, green, blue and white values into a
             single "packed" 32-bit WRGB color.
    @param   r  Red brightness, 0 to 255.
    @param   g  Green brightness, 0 to 255.
    @param   b  Blue brightness, 0 to 255.
    @param   w  White brightness, 0 to 255.
    @return  32-bit packed WRGB value, which can then be assigned to a
             variable for later use or passed to the setPixelColor()
             function. Packed WRGB format is predictable, regardless of
             LED strand color order.
  */
  static uint32_t   WS2812_Color_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
  
  // Input a value 0 to 255 to get a color value.
  // The colors are a transition r - g - b - back to r.
  static uint32_t WS2812_Wheel(uint8_t WheelPos) {
	  WheelPos = 255 - WheelPos;
	  if (WheelPos < 85) {
		  return WS2812_Color_RGBW(255 - WheelPos * 3, 0, WheelPos * 3, 0);
	  }
	  if (WheelPos < 170) {
		  WheelPos -= 85;
		  return WS2812_Color_RGBW(0, WheelPos * 3, 255 - WheelPos * 3, 0);
	  }
	  WheelPos -= 170;
	  return WS2812_Color_RGBW(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
  }
  
 __attribute__ ((unused)) static uint32_t WS2812_gamma32(uint32_t x) {
	 uint8_t *y = (uint8_t *)&x;
	 // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
	 // to avoid a bunch of shifting and masking that would be necessary for
	 // properly handling different endianisms (and each byte is a fairly
	 // trivial operation, so it might not even be wasting cycles vs a check
	 // and branch for the RGB case). In theory this might cause trouble *if*
	 // someone's storing information in the unused most significant byte
	 // of an RGB value, but this seems exceedingly rare and if it's
	 // encountered in reality they can mask values going in or coming out.
	 for(uint8_t i=0; i<4; i++) y[i] = WS2812_gamma8(y[i]);
	 return x; // Packed 32-bit return
}
  
#ifdef __cplusplus
}
#endif

#endif /* WS2812_DRIVER_H_ */