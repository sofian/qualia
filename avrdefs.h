/*
 * avrdefs.h
 *
 * Standard definitions for AVR systems. Allows the use of new, delete,
 * templates, virtual inheritance and purely virtual methods.
 *
 * Based on the following original source code:
 * http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=410870
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
#ifndef AVRDEFS_H_
#define AVRDEFS_H_

#if defined(__AVR__) && defined(__cplusplus)

#include <avr/interrupt.h> // for cli()
#include <stdlib.h>        // for malloc and free

// Operators new / delete.
void * operator new(size_t size);
void operator delete(void * ptr);
void * operator new[](size_t size);
void operator delete[](void * ptr);

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" {
  // Allow the use of templates, virtual inheritance, etc.
  int __cxa_guard_acquire(__guard *g);
  void __cxa_guard_release (__guard *g);
  void __cxa_guard_abort (__guard *);

  // Allows purely virtual functions.
  void __cxa_pure_virtual();
}

#endif

#endif /* AVRDEFS_H_ */
