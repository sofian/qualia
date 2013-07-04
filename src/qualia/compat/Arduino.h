/**
 * This is a port of the Arduino.h file from the Arduino distribution. It ports some of the basic
 * functionalities of the Arduino, making them available on non-Arduino AVR as well as computer
 * platforms.
 */
#ifndef Arduino_Compat_h
#define Arduino_Compat_h

#include <qualia/core/common.h>

#if !is_arduino()

#include <stdlib.h>
#include <string.h>
#include <math.h>

#if is_avr()
#include "new.h"
#endif

#include "binary.h"

#ifndef true
  #define true 0x1
#endif

#ifndef false
  #define false 0x0
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#ifdef __GNUC__
  // min/max are undefined in bits/c++config.h so we define them as templates
  // note that this might yield different results than what you will get on Arduino (which uses macros)
  template<typename T> T min(T a, T b) { return a < b ? a : b; }
  template<typename T> T max(T a, T b) { return a > b ? a : b; }
#else
  #define min(a,b) ((a)<(b)?(a):(b))
  #define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef WIN32
  #ifdef abs
    #undef abs
  #endif
  #define abs(x) ((x)>=0?(x):-(x))
#endif

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean;
typedef uint8_t byte;

#ifdef __cplusplus
extern "C"{
#endif

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

#ifdef __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus

#include "Stream.h" // in replacement of #include "HardwareSerial.h"

// WMath prototypes

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#endif

#if is_computer()
// PROGMEM PSTR() macro compat
#define PSTR(x) (x)
#endif

#endif // !is_arduino()

#endif
