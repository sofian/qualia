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
 *  \file   ParityPrimits.hpp
 *  \brief  Definition parity primitives.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 19:49:22 $
 */


#ifndef ParityPrimits_hpp
#define ParityPrimits_hpp

#include "puppy/Puppy.hpp"


/*!
 *  \class And ParityPrimits.hpp "ParityPrimits.hpp"
 *  \brief Logical AND of two Boolean value GP primitive.
 *  \ingroup Parity
 */
class And : public Puppy::Primitive {

public:
           And();
  virtual ~And() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Or ParityPrimits.hpp "ParityPrimits.hpp"
 *  \brief Logical OR of two Boolean value GP primitive.
 *  \ingroup Parity
 */
class Or : public Puppy::Primitive {

public:
           Or();
  virtual ~Or() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Xor ParityPrimits.hpp "ParityPrimits.hpp"
 *  \brief Exclusive OR of two Boolean value GP primitive.
 *  \ingroup Parity
 */
class Xor : public Puppy::Primitive {

public:
           Xor();
  virtual ~Xor() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


/*!
 *  \class Not ParityPrimits.hpp "ParityPrimits.hpp"
 *  \brief Negation of a Boolean value GP primitive.
 *  \ingroup Parity
 */
class Not : public Puppy::Primitive {

public:
           Not();
  virtual ~Not() { }

  virtual void execute(void* outDatum, Puppy::Context& ioContext);

};


#endif // ParityPrimits_hpp

