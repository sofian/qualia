/*
 * error.h
 *
 * Error/warning/notification routines/macros.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * This file was originally part of Drone (http://drone.ws)
 *   (c) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * Error messages code adapted from Torch
 *   (c) 2004 Ronan Collobert (collober@idiap.ch)
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
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

/**
 * Error/warning/notification routines/macros. Level of debugging can be set by defining
 * DEBUG_LEVEL at compile time. Assertions and messages that are below the level will simply
 * be erased by the preprocessor (macros are actually replaced by a dummy/void operation),
 * thus saving on memory usage and execution time.
 *
 * WARNING: Notice that DEBUG_LEVEL is not supported on AVR/Arduino ie. all error macros are disabled
 * on such platforms.
 */

#include <qualia/core/common.h>

// # Macro definitions #####################################################

// Force DEBUG_LEVEL_NODEBUG when running in AVR/Arduino mode
#if (! is_computer())
  #ifdef DEBUG_LEVEL
    #warning "DEBUG_LEVEL is not supported on AVR/Arduino: forcing DEBUG_LEVEL_NODEBUG"
    #undef DEBUG_LEVEL
  #endif
  #define DEBUG_LEVEL DEBUG_LEVEL_NODEBUG
#endif

// Debug levels.

//! No debug.
#define DEBUG_LEVEL_NODEBUG -1

//! Only catch errors.
#define DEBUG_LEVEL_ERROR   0

//! Catch both errors and warnings.
#define DEBUG_LEVEL_WARNING 1

//! Catch errors, warnings and notices.
#define DEBUG_LEVEL_NOTICE  2

// Default debug level.
#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL DEBUG_LEVEL_ERROR
#endif

// Quick condition checkers.
#define DEBUG_ERROR   (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define DEBUG_WARNING (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define DEBUG_NOTICE  (DEBUG_LEVEL >= DEBUG_LEVEL_NOTICE)

// Redefine __STRING just to make sure.
#ifndef __STRING
#define __STRING(x) #x
#endif

// This macro is absolutely NOT SAFE! NEVER USE IT ALONE!!!
// Use ASSERT_ERROR, ASSERT_WARNING and ASSERT_NOTICE instead, below.
#define __DEBUG_TRIGGER_ASSERT(expr, func) \
((expr) ? static_cast<void>(0) : func("Fail: " __STRING(expr)) );

// Dummy instruction (does nothing).
#define __DEBUG_DUMMY_INSTRUCTION (static_cast<void>(0))

//#define dummymsg static_cast<void>(0);

// # Message functions #####################################################

// # Assertions and messages functions #####################################

#if DEBUG_ERROR

  /// Prints an error message. The program will exit.
  void    Q_ERROR(const char* msg, ...);

  /// Assert expression: if it is false, prints error message and exits.
  void    Q_ASSERT_ERROR_MESSAGE(bool expr, const char* msg, ...);

  /// Asserts expression: if it is false, prints the expression and exits.
  #define Q_ASSERT_ERROR(expr) __DEBUG_TRIGGER_ASSERT(expr, Q_ERROR)

#else

  #define Q_ERROR(...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_ERROR_MESSAGE(expr, ...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_ERROR(expr) __DEBUG_DUMMY_INSTRUCTION

#endif

#if DEBUG_WARNING

  /// Prints warning message.
  void Q_WARNING(const char* msg, ...);

  /// Assert expression: if it is false, prints warning message.
  void Q_ASSERT_WARNING_MESSAGE(bool expr, const char* msg, ...);

  /// Asserts expression: if it is false, prints the expression as a warning message.
  #define Q_ASSERT_WARNING(expr) __DEBUG_TRIGGER_ASSERT(expr, Q_WARNING)

#else

  #define Q_WARNING(...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_WARNING_MESSAGE(expr, ...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_WARNING(expr) __DEBUG_DUMMY_INSTRUCTION

#endif

#if DEBUG_NOTICE

  /// Prints a notice, usually intended for deep information at the programmer's intent.
  void Q_NOTICE(const char* msg, ...);

  /// Assert expression: if it is false, prints message as a notice.
  void Q_ASSERT_NOTICE_MESSAGE(bool expr, const char* msg, ...);

  /// Asserts expression: if it is false, prints the expression as a notice message.
  #define Q_ASSERT_NOTICE(expr) __DEBUG_TRIGGER_ASSERT(expr, Q_NOTICE)

#else

  #define Q_NOTICE(...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_NOTICE_MESSAGE(expr, ...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_ASSERT_NOTICE(expr) __DEBUG_DUMMY_INSTRUCTION

#endif

#if (is_computer())
  /// Same as printf.
  void Q_PRINT(const char* msg, ...);

  /// Prints a formatted message.
  void Q_MESSAGE(const char* msg, ...);
#else
  #define Q_PRINT(...) __DEBUG_DUMMY_INSTRUCTION
  #define Q_MESSAGE(...) __DEBUG_DUMMY_INSTRUCTION
#endif

#endif
