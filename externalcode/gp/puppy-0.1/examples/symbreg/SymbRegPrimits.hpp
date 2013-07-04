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
 *  \file   SymbRegPrimits.hpp
 *  \brief  Definition symbolic regression primitives.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 19:49:22 $
 */


#ifndef SymbRegPrimits_hpp
#define SymbRegPrimits_hpp

#include "puppy/Puppy.hpp"


/*!
 *  \class Add SymbRegPrimits.hpp "SymbRegPrimits.hpp"
 *  \brief Add two doubles GP primitive.
 *  \ingroup SymbReg
 */
class Add : public Puppy::Primitive {

public:
           Add();
  virtual ~Add() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Subtract SymbRegPrimits.hpp "SymbRegPrimits.hpp"
 *  \brief Subtract two doubles GP primitive.
 *  \ingroup SymbReg
 */
class Subtract : public Puppy::Primitive {

public:
           Subtract();
  virtual ~Subtract() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Multiply SymbRegPrimits.hpp "SymbRegPrimits.hpp"
 *  \brief Multiply two doubles GP primitive.
 *  \ingroup SymbReg
 */
class Multiply : public Puppy::Primitive {

public:
           Multiply();
  virtual ~Multiply() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Divide SymbRegPrimits.hpp "SymbRegPrimits.hpp"
 *  \brief Protected division of two doubles GP primitive.
 *  \ingroup SymbReg
 */
class Divide : public Puppy::Primitive {

public:
           Divide();
  virtual ~Divide() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Ephemeral SymbRegPrimits.hpp "SymbRegPrimits.hpp"
 *  \brief Ephemeral random constant.
 *  \ingroup SymbReg
 */
class Ephemeral : public Puppy::Primitive {

public:
           Ephemeral();
  virtual ~Ephemeral() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);
  virtual Puppy::PrimitiveHandle giveReference(Puppy::Context& ioContext);
  
};



#endif // SymbRegPrimits_hpp

