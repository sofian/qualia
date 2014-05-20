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

#if is_computer()
#include <new>
#endif

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
  virtual void* calloc(size_t num, size_t size);
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
 *
 * int* val = (int*) Alloc::malloc(10*sizeof(int));
 * MyClass* obj = Q_NEW(MyClass)(1, 2);    // MyClass* bbj = new MyClass(1,2);
 * MyClass* arr = Q_ARRAY_NEW(MyClass,10); // MyClass* arr = new MyClass[10];
 * ...
 * Alloc::free(val);
 * Q_DELETE(MyClass, ptr);
 * Q_ARRAY_DELETE(arr);
 * @endcode
*/
class Alloc {
  static Allocator* inst;

public:
  static void* malloc(size_t size);
  static void* calloc(size_t num, size_t size);
  static void* realloc(void* ptr, size_t size);
  static void free(void* ptr);
  static void init(Allocator* alloc);
  static Allocator* instance() { return inst; }
};

/// Macro equivalent of *new* operator: allocates memory using Alloc::malloc().
#define Q_NEW(T) new( Alloc::malloc(sizeof(T)) ) T

//void * operator new(size_t size, Allocator* alloc);
//#define Q_NEW(T) new( Alloc::instance() ) T
//#define _NEW new( Alloc::instance() )

//template<class T> T* _Q_ALLOC() { return Alloc::malloc(sizeof(T)); }


/// Template equivalent of *delete* operator: explicitely calls destuctor and releases memory using Alloc::free().
template<class T>
void Q_DELETE(T* obj) {
  obj->~T(); Alloc::free(obj);
}

template<class T>
T* __Q_ARRAY_NEW(size_t n) {
  size_t* ptr = (size_t*) Alloc::malloc(2*sizeof(size_t) + n * sizeof(T));
  *ptr++ = n;
  *ptr++ = sizeof(T);
  return new( ptr ) T[n];
}

/**
 * Macro equivalent of *new[]* operator: allocates memory using Alloc::malloc().
 * Notice that this method allocates an extra (size_t) space to retrieve the number of
 * allocated elements when calling Q_ARRAY_DELETE().
 */
#define Q_ARRAY_NEW(T,n) __Q_ARRAY_NEW<T>(n)

/**
 * Template equivalent of *delete[]* operator: explicitely calls destuctors and releases memory
 * using Alloc::free().
 */
template<class T>
void Q_ARRAY_DELETE(T* array) {
  size_t* ptr = (size_t*) array;
  ptr-=2;
  size_t n = ptr[0];
  size_t elemSize = ptr[1];
  unsigned char* iter = (unsigned char*)array;
  while (n--) {
    ((T*)iter)->~T();
    iter += elemSize;
  }
  Alloc::free( ptr );
}

#define Q_ARRAY(T, n) (T*)Alloc::calloc((size_t)n, sizeof(T))
#define Q_FREE(ptr) Alloc::free(ptr)

#define Q_ARRAY_CLEAR(ptr, T, n) memset(ptr, 0, sizeof(T)*(size_t)n)
#define Q_ARRAY_COPY(to, from, T, n) memcpy(to, from, sizeof(T)*(size_t)n);

#endif /* ALLOCATOR_H_ */
