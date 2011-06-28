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
#define STATIC_ALLOCATOR_SIZE 100

#ifndef STATIC_ALLOCATOR_INC
#define STATIC_ALLOCATOR_INC

unsigned char static_buffer[STATIC_ALLOCATOR_SIZE];
unsigned int static_buffer_idx = 0;

void * static_malloc(size_t size) {
  if (static_buffer_idx + size > STATIC_ALLOCATOR_SIZE)
    return NULL;
  unsigned char* tmp = (static_buffer + static_buffer_idx);
  static_buffer_idx += size;
  return (void*)tmp;
}

#endif
