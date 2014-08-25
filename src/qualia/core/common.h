/*
 * common.h
 *
 * Common definitions.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * This file provides some common includes and definition. Among other things,
 * it gives access to some common Arduino functions when compiling on a computer.
 * This file should be included at the top of every header file that uses Qualia.
 */
#ifndef COMMON_H_
#define COMMON_H_

// Platform check macros.

/// True iff we are compiling for AVR.
#define is_avr()      defined(__AVR__)

/// True iff we are compiling for Arduino (notice that is_arduino() implies is_avr())
#define is_arduino()  defined(ARDUINO)

/// True iff we are compiling for a non-AVR platform.
#define is_computer() !defined(__AVR__)

// Define WIN32 (for liblo)
#ifdef _WIN32
  #ifndef WIN32
    #define WIN32
  #endif
#endif

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>

// Old systems need that to define FLT_MAX and DBL_MAX
#ifndef DBL_MAX
  #if !is_computer() or defined(_WIN32)
    #include <float.h>
  #else
    #include <values.h>
  #endif
#endif

#ifdef _MSC_VER
  #ifndef for
    #define for if (0) {} else for
  #endif

  #define M_PI 3.14159265358979323846
  #define popen(x,y) _popen(x,y)
  #define pclose(x) _pclose(x)
  #define isnan(x) _isnan(x)
  #define log1p(x) log(1+(x))
#endif

// Basic definitions from Arduino.h
#if (is_arduino())
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
#endif


/// Flips bit #bit# in element #value#.
#define bitFlip(value, bit) ((value) ^= (1UL << (bit))) // Added an extra bit macro.

// Parameters.
//#define USE_DOUBLE
//#define USE_STATIC_ALLOC
//#define STATIC_ALLOCATOR_SIZE 100

//#define DEBUG_LEVEL 2

#ifdef USE_DOUBLE
  typedef double real;
  #define REAL_MAX DBL_MAX
  #define REAL_MIN DBL_MIN
  #define INF DBL_MAX
#else
  typedef float real;
  #define REAL_MAX FLT_MAX
  #define REAL_MIN FLT_MIN
  #define INF FLT_MAX
#endif

  // We use srandom/random/RANDOM_MAX instead of srand/rand/RAND_MAX
#ifdef WIN32
  #define srandom srand
  #define random rand
#endif

#ifndef RANDOM_MAX
  #define RANDOM_MAX RAND_MAX
#endif

#if !is_arduino()
#include <qualia/compat/Arduino.h>
#endif

#if is_avr()
#include <qualia/compat/placement_new.h>
#include <qualia/compat/computer.h>
#endif

#include <qualia/core/error.h>
#include <qualia/core/Allocator.h>

#endif
