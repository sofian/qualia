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
 *  \file   puppy/TokenT.hpp
 *  \brief  Definition of the template class TokenT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2004/02/11 14:23:03 $
 */

#ifndef Puppy_TokenT_hpp
#define Puppy_TokenT_hpp

#include <map>
#include <string>
#include <vector>

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/PrimitiveInline.hpp"
#include "puppy/Context.hpp"
#include "puppy/Tree.hpp"

namespace Puppy {


/*!
 *  \class TokenT puppy/TokenT.hpp "puppy/TokenT.hpp"
 *  \brief Token template, to use as variable terminal primitive.
 *  \param T Templated type.
 *  \ingroup Puppy
 */
template <class T>
class TokenT : public Primitive {

public:

  explicit TokenT(std::string inName="TOKEN");
  explicit TokenT(std::string inName, const T& inToken);
  virtual ~TokenT() { }

  virtual void execute(void* outDatum, Context& ioContext);
  virtual void setValue(const void* inValue);

  T mToken;   //!< Token value of the primitive.
      
};

}


/*!
 *  \brief Construct a token primitive.
 *  \param inName Name of the primitive.
 */
template <class T>
Puppy::TokenT<T>::TokenT(std::string inName) :
  Puppy::Primitive(0, inName)
{ }


/*!
 *  \brief Construct a token primitive.
 *  \param inName Name of the primitive.
 *  \param inToken Value of the token.
 */
template <class T>
Puppy::TokenT<T>::TokenT(std::string inName, const T& inToken) :
  Puppy::Primitive(0, inName),
  mToken(inToken)
{ }


/*!
 *  \brief Execute the caracteristic token primitive operation.
 *  \param outDatum Result of the execution.
 *  \param ioContext Evolutionary context.
 */
template <class T>
void Puppy::TokenT<T>::execute(void* outDatum, Puppy::Context& ioContext)
{
  T& lResult = *(T*)outDatum;
  lResult = mToken;
}


/*!
 *  \brief Set the value of the token primitive.
 *  \param inValue Value of the token.
 */
template <class T>
void Puppy::TokenT<T>::setValue(const void* inValue)
{
  mToken = *(const T*)inValue;
}


#endif // Puppy_Context_hpp
