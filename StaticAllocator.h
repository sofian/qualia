/*
 * StaticAllocator.h
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

#ifndef STATIC_ALLOCATOR_SIZE
#define STATIC_ALLOCATOR_SIZE 100
#endif

class StaticAllocator: public Allocator {
  static unsigned char buffer[STATIC_ALLOCATOR_SIZE];
  static unsigned int  bufferIdx;

public:
  StaticAllocator() {}

protected:
  virtual void* malloc(size_t size);
  virtual void free(void* ptr) {}
};

#endif /* STATICALLOCATOR_H_ */
