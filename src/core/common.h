/*
 * common.h
 *
 * Common type definitions.
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
#ifndef COMMON_H_
#define COMMON_H_

#if defined(__cplusplus)
extern "C" {
#endif

// Platform check.
#define is_avr() defined(__AVR__)
#define is_arduino() defined(ARDUINO)
#define is_computer() !defined(__AVR__)

#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

// Old systems need that to define FLT_MAX and DBL_MAX
#ifndef DBL_MAX
#include <values.h>
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
#if (!is_arduino())

// Make sure to redefine the min/max/abs macros
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef abs
#undef abs
#endif

// XXX Since
//#ifdef round
//#undef round
//#endif

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

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>=0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
#endif

// Parameters.
//#define USE_DOUBLE
//#define USE_STATIC_ALLOC
//#define STATIC_ALLOCATOR_SIZE 100

//#define DEBUG_LEVEL 2

#ifdef USE_DOUBLE
typedef double real;
#define INF DBL_MAX
#else
typedef float real;
#define INF FLT_MAX
#endif

#if is_avr()
#include "avrdefs.h"
#endif

#if defined(__cplusplus)
}
#endif

#ifdef DEBUG
#include <core/error.h>
#endif

//#include "error.h"
#include "Allocator.h"
//#include "Action.h"
//#include "Observation.h"

#endif
