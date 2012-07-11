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


// Parameters.
//#define USE_DOUBLE
//#define USE_STATIC_ALLOC
//#define STATIC_ALLOCATOR_SIZE 100

#define DEBUG_LEVEL 2

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

#include "error.h"
#include "Allocator.h"
#include "Action.h"
#include "Observation.h"

#endif
