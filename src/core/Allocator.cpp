/*
 * Allocator.cpp
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
#include "Allocator.h"
#include <stdlib.h>


void* Allocator::malloc(size_t size) {
  return ::malloc(size);
}

void Allocator::free(void* ptr) {
  if (ptr) ::free(ptr);
}

Allocator* Alloc::inst = 0;

void* Alloc::malloc(size_t size) {
  if (!inst)
    return ::malloc(size);
  else
    return inst->malloc(size);
}

void Alloc::free(void* ptr) {
  if (!inst)
    ::free(ptr);
  else
    inst->free(ptr);
}

void Alloc::init(Allocator* alloc) {
  inst = alloc;
}

void* operator new(size_t size, Allocator& alloc)
{
  return alloc.malloc(size);
}

void operator delete(void* ptr, Allocator& alloc)
{
  alloc.free(ptr);
}

