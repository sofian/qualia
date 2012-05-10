/*
 * avrdefs.cpp
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

#include "avrdefs.h"

#if defined(__AVR__) && defined(__cplusplus)

// Operators new / delete.
void * operator new(size_t size) { return malloc(size); }
void operator delete(void * ptr) { if (ptr) free(ptr); }
void * operator new[](size_t size) { return malloc(size); }
void operator delete[](void * ptr) { if (ptr) free(ptr); }

extern "C" {
  // Allow the use of templates, virtual inheritance, etc.
  int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
  void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
  void __cxa_guard_abort (__guard *) {};

  // Allows purely virtual functions.
  void __cxa_pure_virtual() { cli(); for (;;); }
}
#else
// HACK: Prevents "/usr/bin/ranlib: file: libqualia.a(avrdefs.o) has no symbols" errors.
char __avrdefsDummyInclude;
#endif
