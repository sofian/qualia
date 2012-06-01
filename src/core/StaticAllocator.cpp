/*
 * StaticAllocator.cpp
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

#include "StaticAllocator.h"

StaticAllocator::StaticAllocator(unsigned char* _buffer, size_t size) :
  buffer(_buffer), bufferSize(size), bufferIdx(0), nLeaks(0) { }

void* StaticAllocator::malloc(size_t size) {
  if (bufferIdx + size > bufferSize)
    return NULL;
  unsigned char* tmp = (buffer + bufferIdx);
  bufferIdx += size;
  return (void*)tmp;
}

void StaticAllocator::free(void* ptr) {
  nLeaks++;
  lastLeak = (unsigned char*) ptr;
}

void StaticAllocator::freeAll() {
  bufferIdx = 0;
  nLeaks = 0;
}
