/**
 * @section LICENSE
 *  Copyright (c) 2012 Chris Dick
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *  This is a library for allowing the use of programs written for the Arduino
 *  with the mbed. This was started for use with the Gameduino shield Library
 *
 * @file "arduino.h"
 */
#ifndef ARDUINO_H
#define ARDUINO_H

#include "mbed.h"
#include "math.h"
// Macros

#define pgm_read_word(x)          (*(const short int*)x) 
#define pgm_read_dword_near(x)    (*(const int*)x)
#define pgm_read_word_near(x)     (*(const unsigned int*)x)
#define pgm_read_int_near(x)      (*(const int*)x)
#define pgm_read_int(x)           (*(const int*)x)
#define pgm_read_byte(x)          (*(const char*)x)
#define pgm_read_byte_near(x)     (*(const char*)x)
#define PROGMEM                   const
#define char(x)                   ((char)x)
#define byte(x)                   ((byte)x)
#define int(x)                    ((int)x)
#define word(x)                   ((word)x)
#define long(x)                   ((long)x)
#define float(x)                  ((float)x)

/** Macro for delay() 
 *
 * @param void
 */
#define delay(x)                  (wait_ms(x))
/** Macro for delayMicroseconds()  
 *
 * @param void
 */
#define delayMicroseconds(x)      (wait_us(x))

/** Macro for min() 
 *
 * @param any
 */
#define min(a,b)                  ((a)<(b)?(a):(b))
/** Macro for max()
 *
 * @param any
 */
#define max(a,b)                  ((a)>(b)?(a):(b))
/** Macro for abs()
 *
 * @param any
 */
#define abs(x)                    ((x)>0?(x):(x*-1))

/** Macro for randomSeed()
 *
 * @param int
 */
#define randomSeed(x)             srand(x)


// typedefs

typedef unsigned char prog_uint8_t;
typedef unsigned int prog_uint16_t;
typedef unsigned int prog_uint32_t;
typedef unsigned char byte;
typedef bool boolean;
typedef unsigned char prog_uchar;
typedef signed char prog_char;
typedef signed long int word;

// function prototypes

void timer_start(void);
long millis(void);
long micros(void);
byte lowByte(short int low);
byte highByte(short int high);
int random(int number);
int random(int numberone, int numbertwo);
#endif
