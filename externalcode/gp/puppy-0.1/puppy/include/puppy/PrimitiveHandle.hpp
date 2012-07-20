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
 *  \file   puppy/PrimitiveHandle.hpp
 *  \brief  Definition of the class PrimitiveHandle.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2004/02/11 14:23:03 $
 */

#ifndef Puppy_PrimitiveHandle_hpp
#define Puppy_PrimitiveHandle_hpp

#include "iostream"

#include "puppy/PrimitiveHandle.hpp"


namespace Puppy {

class Primitive;  // Forward declaration

/*!
 *  \class PrimitiveHandle puppy/PrimitiveHandle.hpp "puppy/PrimitiveHandle.hpp"
 *  \brief Smart pointer to a primitive.
 *  \ingroup Puppy
 *
 *  Primitive handle defines a smart pointer to a primitive. It behaves much like
 *  a standard pointer, but it also call the appriate refer and unrefer methods of
 *  class primitive.
 */
class PrimitiveHandle {

public:

  inline PrimitiveHandle();
  inline PrimitiveHandle(const Primitive* inPrimitPtr);
  inline PrimitiveHandle(const PrimitiveHandle& inPrimitiveHandle);
  inline ~PrimitiveHandle();

  inline PrimitiveHandle& operator=(const Primitive* inPrimitPtr);
  inline PrimitiveHandle& operator=(const PrimitiveHandle& inPrimitiveHandle);
  inline Primitive&       operator*();
  inline const Primitive& operator*() const;
  inline Primitive*       operator->();
  inline const Primitive* operator->() const;
  inline bool             operator!() const;
  inline bool             operator==(const Primitive* inPrimitPtr) const;
  inline bool             operator==(const PrimitiveHandle& inPrimitiveHandle) const;
  inline bool             operator!=(const Primitive* inPrimitPtr) const;
  inline bool             operator!=(const PrimitiveHandle& inPrimitiveHandle) const;
  
  inline Primitive*       getPointer();
  inline const Primitive* getPointer() const;

private:
  Primitive* mPrimitivePtr;  // Reference to the associated primitive.
   
};

}

#endif // Puppy_PrimitiveHandle_hpp


 
