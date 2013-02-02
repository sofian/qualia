/*
 *  BEAGLE Puppy
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   puppy/PrimitiveInline.hpp
 *  \brief  Implementation Primitive and PrimitiveHandle inline functions.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3 $
 *  $Date: 2004/02/11 19:49:22 $
 */

#ifndef Puppy_PrimitiveInline_hpp
#define Puppy_PrimitiveInline_hpp

#include <cassert>

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/Context.hpp"
#include "puppy/Tree.hpp"


/*!
 *  \brief Construct a NULL pointing primitive smart PrimitiveHandle.
 */
inline Puppy::PrimitiveHandle::PrimitiveHandle() :
  mPrimitivePtr(NULL)
{ }


/*!
 *  \brief Construct a primitive smart pointer that refer to the object pointed given.
 *  \param inPrimitPtr Pointer to the object to be referenced.
 */
inline Puppy::PrimitiveHandle::PrimitiveHandle(const Puppy::Primitive* inPrimitPtr) :
  mPrimitivePtr( (inPrimitPtr!=NULL) ? (const_cast<Primitive*>(inPrimitPtr)->refer()) : NULL )
{ }


/*!
 *  \brief Copy construct a primitive smart pointer.
 *  \param inPrimitiveHandle Smart PrimitiveHandle to copy.
 */
inline Puppy::PrimitiveHandle::PrimitiveHandle(const Puppy::PrimitiveHandle& inPrimitiveHandle) :
  mPrimitivePtr( (inPrimitiveHandle.getPointer()!=NULL) ?
    (const_cast<Primitive*>(inPrimitiveHandle.getPointer())->refer()) : NULL )
{ }

  
/*!
 *  \brief Destruct a primitive smart pointer by unrefering the pointed object.
 */
inline Puppy::PrimitiveHandle::~PrimitiveHandle()
{
  if(mPrimitivePtr!=NULL) mPrimitivePtr->unrefer();
  mPrimitivePtr = NULL;
}


/*!
 *  \brief Affect the actual PrimitiveHandle to an object PrimitiveHandle.
 *  \param inPrimitPtr PrimitiveHandle to the object to refer.
 *  \return Actual smart PrimitiveHandle.
 */
inline Puppy::PrimitiveHandle& Puppy::PrimitiveHandle::operator=(const Puppy::Primitive* inPrimitPtr)
{
  if(mPrimitivePtr==inPrimitPtr) return *this;
  if(mPrimitivePtr!=NULL) mPrimitivePtr->unrefer();
  if(inPrimitPtr) mPrimitivePtr = const_cast<Primitive*>(inPrimitPtr)->refer();
  else mPrimitivePtr = NULL;
  return *this;
}


/*!
 *  \brief Affect the actual pointer to another smart pointer.
 *  \param inPrimitiveHandle Smart pointer to copy.
 *  \return Actual smart pointer.
 */
inline Puppy::PrimitiveHandle&
Puppy::PrimitiveHandle::operator=(const Puppy::PrimitiveHandle& inPrimitiveHandle)
{
  if(this==&inPrimitiveHandle) return *this;
  if(mPrimitivePtr==inPrimitiveHandle.getPointer()) return *this;
  if(mPrimitivePtr) mPrimitivePtr->unrefer();
  if(inPrimitiveHandle.getPointer()!=NULL)
    mPrimitivePtr = const_cast<Primitive*>(inPrimitiveHandle.getPointer())->refer();
  else mPrimitivePtr = NULL;
  return *this;
}


/*!
 *  \brief Get reference the primitive pointed.
 *  \return Reference to the primitive pointed.
 */
inline Puppy::Primitive& Puppy::PrimitiveHandle::operator*()
{
  assert(mPrimitivePtr != NULL);
  return *mPrimitivePtr;
}


/*!
 *  \brief Get constant reference the primitive pointed.
 *  \return Constant reference to the primitive pointed.
 */
inline const Puppy::Primitive& Puppy::PrimitiveHandle::operator*() const
{
  assert(mPrimitivePtr != NULL);
  return *mPrimitivePtr;
}


/*!
 *  \brief  Get reference the primitive pointed.
 *  \return PrimitiveHandle to the object pointed.
 */
inline Puppy::Primitive* Puppy::PrimitiveHandle::operator->()
{
  assert(mPrimitivePtr != NULL);
  return mPrimitivePtr;
}


/*!
 *  \brief  Get constant reference the primitive pointed.
 *  \return Constant pointer to the primitive pointed.
 */
inline const Puppy::Primitive* Puppy::PrimitiveHandle::operator->() const
{
  assert(mPrimitivePtr != NULL);
  return mPrimitivePtr;
}


/*!
 *  \brief  Test whether the pointer is NULL or not.
 *  \return True if the pointer is NULL, false if it is not.
 */
inline bool Puppy::PrimitiveHandle::operator!() const
{
  return (!mPrimitivePtr);
}


/*!
 *  \brief Compare a smart pointer to a primitive pointer.
 *  \param inPrimitPtr Right hand side primitive pointer to compare.
 *  \return True if they both refer to the same object (or are both NULL), false if not.
 */
inline bool Puppy::PrimitiveHandle::operator==(const Puppy::Primitive* inPrimitPtr) const
{
  return (mPrimitivePtr == inPrimitPtr);
}


/*!
 *  \brief Compare two primitive smart pointer.
 *  \param inPrimitiveHandle Right hand side smart pointer to compare.
 *  \return True if they both refer to the same object (or are both NULL), false if not.
 */
inline bool Puppy::PrimitiveHandle::operator==(const Puppy::PrimitiveHandle& inPrimitiveHandle) const
{
  return (mPrimitivePtr == inPrimitiveHandle.getPointer());
}


/*!
 *  \brief Compare a smart pointer to a primitive pointer.
 *  \param inPrimitPtr Right hand side primitive pointer to compare.
 *  \return False if they both refer to the same object (or are both NULL), true if not.
 */
inline bool Puppy::PrimitiveHandle::operator!=(const Puppy::Primitive* inPrimitPtr) const
{
  return (mPrimitivePtr != inPrimitPtr);
}


/*!
 *  \brief Compare two smart PrimitiveHandles.
 *  \param inPrimitiveHandle Right hand side smart PrimitiveHandle to compare.
 *  \return False if they both refer to the same object (or are both NULL), true if not.
 */
inline bool Puppy::PrimitiveHandle::operator!=(const Puppy::PrimitiveHandle& inPrimitiveHandle) const
{
  return (mPrimitivePtr != inPrimitiveHandle.getPointer());
}


/*!
 *  \brief Return pointer to the referenced primitive.
 *  \return C++ pointer to the primitive.
 */
inline Puppy::Primitive* Puppy::PrimitiveHandle::getPointer()
{
  return mPrimitivePtr;
}


/*!
 *  \brief Return constant pointer to the referenced primitive.
 *  \return Constant C++ pointer to the primitive.
 */
inline const Puppy::Primitive* Puppy::PrimitiveHandle::getPointer() const
{
  return mPrimitivePtr;
}


/*!
 *  \brief Get the name of the primitive.
 *  \return Name of the primitive.
 */
inline std::string Puppy::Primitive::getName() const
{
  return mName;
}


/*!
 *  \brief Get the number of arguments of the primitive.
 *  \return Number of arguments of the primitive.
 */
inline unsigned int Puppy::Primitive::getNumberArguments() const
{
  return mNumberArguments;
}


/*!
 *  \brief Increments the reference counter and returns a pointer to the actual primitive.
 *  \return Pointer to the actual primitive.
 */
inline Puppy::Primitive* Puppy::Primitive::refer()
{
  ++mRefCounter;
  return this;
}


/*!
 *  \brief Decrement the reference counter and deletes the actual primitive if it reaches zero.
 */
inline void Puppy::Primitive::unrefer()
{
  if((--mRefCounter) == 0) delete this;
}


/*!
 *  \brief Get the value of the nth argument.
 *  \param inN Index of the argument to get.
 *  \param outResult Value of the nth argument.
 *  \param ioContext Evolutionary context.
 */
inline void
Puppy::Primitive::getArgument(unsigned int inN, void* outResult, Puppy::Context& ioContext)
{
  assert(inN >= 0);
  assert(inN < mNumberArguments);
  unsigned int lIndex = ioContext.mCallStack.back() + 1;
  for(unsigned int i=0; i<inN; ++i) lIndex += (*ioContext.mTree)[lIndex].mSubTreeSize;
  ioContext.mCallStack.push_back(lIndex);
  (*ioContext.mTree)[lIndex].mPrimitive->execute(outResult, ioContext);
  ioContext.mCallStack.pop_back();
}


#endif // Puppy_PrimitiveInline_hpp


 
