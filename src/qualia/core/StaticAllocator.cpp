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
  buffer(_buffer), bufferSize(size), bufferIdx(0), lastPtr(0), nLeaks(0), lastLeak(0) { }

void* StaticAllocator::malloc(size_t size) {
  if (bufferIdx + size > bufferSize)
    return NULL;
  lastPtr = (buffer + bufferIdx); // &buffer[bufferIdx]
  bufferIdx += size;
  return lastPtr;
}

void* StaticAllocator::calloc(size_t num, size_t size) {
  return malloc(size * num);
}

void* StaticAllocator::realloc(void* ptr, size_t size) {
  if (ptr == NULL)
    return malloc(size);
  else if (ptr == lastPtr) { // reallocating last allocated pointer
    //void* currentPtr = (buffer + bufferIdx);
//    bufferIdx -= (unsigned int)(buffer + bufferIdx - lastPtr);
//    Q_ASSERT_WARNING(&buffer[bufferIdx] == lastPtr);
//    bufferIdx += size;
    bufferIdx += (unsigned int)( (buffer + size) - lastPtr); // reassign bufferIdx
    return lastPtr;
  } else {
    Q_WARNING("StaticAllocator:realloc() call will result in memory block lost at %p.", ptr);
    free(ptr);
    return malloc(size);
  }
}

void StaticAllocator::free(void* ptr) {
  nLeaks++;
  lastLeak = (unsigned char*) ptr;
}

void StaticAllocator::freeAll() {
  bufferIdx = 0;
  nLeaks = 0;
}
