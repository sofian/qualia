/*
 * StaticAllocator.h
 *
 * An allocator (see Allocator.h) that "allocates" memory based on a
 * pre-allocated static memory pool/buffer. Useful to manage memory on
 * architectures that don't support well dynamic allocation (such as
 * AVR-based systems). On such systems, it is usually recommended NOT
 * to use dynamic allocation to avoid problems.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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

#ifndef STATICALLOCATOR_H_
#define STATICALLOCATOR_H_

#include "Allocator.h"

/**
 * An allocator (see Allocator) that "allocates" memory based on a
 * pre-allocated static memory pool/buffer. Useful to manage memory on
 * architectures that don't support well dynamic allocation (such as
 * AVR-based systems). On such systems, it is usually recommended NOT
 * to use dynamic allocation to avoid problems. StaticAllocator makes
 * sure the memory is allocated from the stack rather than from the heap.
 *
 * WARNING: Calling StaticAllocator::free() does NOT free the pointer at all. You should in fact
 * NEVER have to call that function (because the memory then becomes completely useless). The
 * object keeps track of any calls to free() by incrementing the nLeaks counter. The variable
 * lastLeak is also updated with the value of the pointer on which free() was called last.
 *
 * NOTE: An alternative is to tune the heap start and end in malloc.
 * http://www.nongnu.org/avr-libc/user-manual/malloc.html
 *
 * Usage:
 * @code
 * unsigned char mybuffer[100];
 * StaticAllocator alloc(mybuffer, 100);
 * int* myarray = (int*)alloc.malloc(10*sizeof(int));
 * Object* myobject = new(alloc.malloc(sizeof(Object)) Object(1,2);
 * @endcode
*/
class StaticAllocator: public Allocator {
public:
  unsigned char* buffer;
  size_t bufferSize;
  unsigned int bufferIdx;
  unsigned char* lastPtr;

  // Keeps track of leaks, for debugging.
  unsigned int nLeaks;
  unsigned char* lastLeak;

public:
  StaticAllocator(unsigned char* buffer, size_t size);

protected:
  virtual void* malloc(size_t size);

  virtual void* calloc(size_t num, size_t size);

  // WARNING: You should not use that method for static allocators unless you know what you are doing.
  virtual void* realloc(void* ptr, size_t size);

  // WARNING: Calling StaticAllocator::free() does NOT free the pointer at all. See note above.
  virtual void free(void* ptr);

  // Frees all pointers.
  virtual void freeAll();
};

#endif /* STATICALLOCATOR_H_ */
