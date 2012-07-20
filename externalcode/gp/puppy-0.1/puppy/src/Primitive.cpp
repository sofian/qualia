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
 *  \file   src/Primitive.cpp
 *  \brief  Implementation of Primitive methods.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 19:49:22 $
 */

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/PrimitiveInline.hpp"
#include "puppy/Randomizer.hpp"
#include "puppy/Context.hpp"
#include "puppy/Tree.hpp"
#include "puppy/TokenT.hpp"


/*!
 *  \brief Construct a primitive.
 *  \param inNumberArguments Number of arguments of the primitive.
 *  \param inName Name of the primitive.
 */
Puppy::Primitive::Primitive(unsigned int inNumberArguments, std::string inName) :
  mName(inName),
  mNumberArguments(inNumberArguments),
  mRefCounter(0)
{ }


/*!
 *  \brief Copy-construct a primitive.
 *  \param inRightPrimit Primitive to copy.
 */
Puppy::Primitive::Primitive(const Primitive& inRightPrimit):
  mName(inRightPrimit.mName),
  mNumberArguments(inRightPrimit.mNumberArguments),
  mRefCounter(0)
{ }


/*!
 *  \brief Copy a primitive.
 *  \param inRightPrimit Primitive to copy.
 */
Puppy::Primitive& Puppy::Primitive::operator=(const Puppy::Primitive& inRightPrimit)
{
  if(this == &inRightPrimit) return *this;
  mName = inRightPrimit.mName;
  mNumberArguments = inRightPrimit.mNumberArguments;
  return *this;
}


/*!
 *  \brief Give a reference on the actual primitive.
 *  \param ioContext Evolutionary context.
 *  \return Primitive handle to this pointer.
 */
Puppy::PrimitiveHandle Puppy::Primitive::giveReference(Puppy::Context& ioContext)
{
  return this;
}


/*!
 *  \brief Set the name of the primitive.
 *  \param inName Name of the primitive.
 */
void Puppy::Primitive::setName(std::string inName)
{
  mName = inName;
}


/*!
 *  \brief Set the number of arguments of the primitive.
 *  \param inNumberArguments Number of arguments of the primitive.
 */
void Puppy::Primitive::setNumberArguments(unsigned int inNumberArguments)
{
  mNumberArguments = inNumberArguments;
}


/*!
 *  \brief Set the value of the primitive (do nothing for basic primitive).
 *  \param inValue New value to use.
 */
void Puppy::Primitive::setValue(const void* inValue)
{ }

