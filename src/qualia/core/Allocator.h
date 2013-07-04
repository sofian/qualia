/*
 * Allocator.h
 *
 * Allows for different implementation of memory allocation methods.
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
#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "common.h"

#include <stddef.h>
#include <stdlib.h>

/**
 * Allows for different implementation of memory allocation methods. This is the parent class, which contains
 * basic implementations of malloc/realloc/free functions from the standard library. Subclasses may override
 * these functions.
*/
class Allocator {
public:
  virtual ~Allocator() {}

  virtual void* malloc(size_t size);
  virtual void* realloc(void* ptr, size_t size);
  virtual void free(void* ptr);
};

/**
 * Static class for memory management. In Qualia, all dynamic memory allocation calls should use the Alloc static
 * methods instead of the ones provided in the standard library. That allows code to be easily ported to AVR platform
 * where static allocation is usually better (see StaticAllocator).
 *
 * Example use (see class StaticAllocator).
 * @code
 * #if is_computer()
 * Allocator allocator; // use standard allocator
 * #else
 * unsigned char STATIC_BUFFER[1000];
 * StaticAllocator allocator(&STATIC_BUFFER); // use static allocator
 * #endif
 *
 * Alloc::init(&allocator);
 * int* val = (int*) Alloc::malloc(10*sizeof(int));
 * MyObject* obj = new(Alloc::instance()) MyObject;
 * ...
 * Alloc::free(val);
 * @endcode
*/
class Alloc {
  static Allocator* inst;

public:
  static void* malloc(size_t size);
  static void* realloc(void* ptr, size_t size);
  static void free(void* ptr);
  static void init(Allocator* alloc);
  static Allocator* instance() { return inst; }
};

void* operator new(size_t size, Allocator* alloc);

#endif /* ALLOCATOR_H_ */
