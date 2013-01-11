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

#include <qualia/core/common.h>

// # Macro definitions #####################################################

// Force NODEBUG when running in AVR/Arduino mode
#if (! is_computer())
  #ifdef DEBUG_LEVEL
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
#define __TRIGGER_ASSERT(expr, func) \
((expr) ? static_cast<void>(0) : func("fail: " __STRING(expr)) );
#define __DUMMY_ASSERT (static_cast<void>(0));

//#define dummymsg static_cast<void>(0);

// # Message functions #####################################################

// Dummy methods (used for empty macros, see up there).
inline void dummymsg(const char* , ...) {}
inline void assertdummymsg(bool, const char* , ...) {}

//! Prints a message.
void message(const char* msg, ...);

//! Like printf.
void print(const char* msg, ...);

//! Prints an error message. The program will exit.
void errormsg(const char* msg, ...);

//! Prints a warning message. The program will not.
void warningmsg(const char* msg, ...);

//! Prints a notice, usually intended for deep information at the programmer's intent.
void noticemsg(const char* msg, ...);

// # Assertions and messages functions #####################################

//! Error messages/assertion.
#if DEBUG_ERROR
  // XXX This was conflicting...
  #define ERROR errormsg
  void ASSERT_ERROR_MESSAGE(bool expr, const char* msg, ...);
  #define ASSERT_ERROR(expr) __TRIGGER_ASSERT(expr, errormsg)
#else
  #define error dummymsg
  #define ASSERT_ERROR(expr) __DUMMY_ASSERT
  #define ASSERT_ERROR_MESSAGE assertdummymsg
#endif

//! Warning messages/assertion.
#if DEBUG_WARNING
  #define WARNING warningmsg
  void ASSERT_WARNING_MESSAGE(bool expr, const char* msg, ...);
  #define ASSERT_WARNING(expr) __TRIGGER_ASSERT(expr, warningmsg)
#else
  #define WARNING dummymsg
  #define ASSERT_WARNING(expr) __DUMMY_ASSERT
  #define ASSERT_WARNING_MESSAGE assertdummymsg
#endif

//! Notice messages/assertion.
#if DEBUG_NOTICE
  #define NOTICE noticemsg
  void ASSERT_NOTICE_MESSAGE(bool expr, const char* msg, ...);
  #define ASSERT_NOTICE(expr) __TRIGGER_ASSERT(expr, noticemsg)
#else
  #define NOTICE dummymsg
  #define ASSERT_NOTICE(expr) __DUMMY_ASSERT
  #define ASSERT_NOTICE_MESSAGE assertdummymsg
#endif


#endif
