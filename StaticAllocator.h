/*
 * StaticAllocator.h
 *
 * An allocator (see Allocator.h) that "allocated" memory based on a
 * pre-allocated static memory pool/buffer. Useful to manage memory on
 * architectures that don't support well dynamic allocation (such as
 * AVR-based systems). On such systems, it is usually recommended NOT
 * to use dynamic allocation to avoid problems.
 *
 * Notice that the free(void*) method doesn't do anything for now.
 *
 * NOTE: An alternative is to tune the heap start and end in malloc.
 * http://www.nongnu.org/avr-libc/user-manual/malloc.html
 *
 * Usage:
 * <code>
 * unsigned char mybuffer[100];
 * StaticAllocator alloc(mybuffer, 100);
 * int* myarray = (int*)alloc.malloc(10*sizeof(int));
 * Object myobject = new(alloc) Object(1,2);
 * </code>
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

class StaticAllocator: public Allocator {
  unsigned char* buffer;
  size_t bufferSize;
  unsigned int bufferIdx;

public:
  StaticAllocator(unsigned char* buffer, size_t size);

protected:
  virtual void* malloc(size_t size);
  virtual void free(void* ptr) {}

  // Frees all pointers.
  virtual void freeAll();
};

#endif /* STATICALLOCATOR_H_ */
