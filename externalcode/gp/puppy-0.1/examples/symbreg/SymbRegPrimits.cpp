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
 *  \file   SymbRegPrimits.cpp
 *  \brief  Implementation symbolic regression primitives.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3 $
 *  $Date: 2004/02/11 18:38:12 $
 */

#include <cmath>
#include <sstream>
 
#include "SymbRegPrimits.hpp"

using namespace Puppy;


/*!
 *  \brief Construct Add GP primitive.
 */
Add::Add() :
  Primitive(2, "ADD")
{ }


/*!
 *  \brief Execute characteristic operation of Add primitive.
 *  \param outDatum Result of the Add operation.
 *  \param ioContext Evolutionary context.
 */
void Add::execute(void* outDatum, Context& ioContext)
{
  double& lResult = *(double*)outDatum;
  double lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult += lArg2;
}


/*!
 *  \brief Construct Subtract GP primitive.
 */
Subtract::Subtract() :
  Primitive(2, "SUB")
{ }


/*!
 *  \brief Execute characteristic operation of Subtract primitive.
 *  \param outDatum Result of the Subtract operation.
 *  \param ioContext Evolutionary context.
 */
void Subtract::execute(void* outDatum, Context& ioContext)
{
  double& lResult = *(double*)outDatum;
  double lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult -= lArg2;
}


/*!
 *  \brief Construct Multiply GP primitive.
 */
Multiply::Multiply() :
  Primitive(2, "MUL")
{ }


/*!
 *  \brief Execute characteristic operation of Multiply primitive.
 *  \param outDatum Result of the Multiply operation.
 *  \param ioContext Evolutionary context.
 */
void Multiply::execute(void* outDatum, Context& ioContext)
{
  double& lResult = *(double*)outDatum;
  double lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult *= lArg2;
}


/*!
 *  \brief Construct Divide GP primitive.
 */
Divide::Divide() :
  Primitive(2, "DIV")
{ }


/*!
 *  \brief Execute characteristic operation of Divide primitive.
 *  \param outDatum Result of the Divide operation.
 *  \param ioContext Evolutionary context.
 */
void Divide::execute(void* outDatum, Context& ioContext)
{
  double& lResult = *(double*)outDatum;
  double lArg2;
  getArgument(1, &lArg2, ioContext);
  if(std::fabs(lArg2) < 0.001) lResult = 1.0;
  else {
    getArgument(0, &lResult, ioContext);
    lResult /= lArg2;
  }
}


/*!
 *  \brief Construct ephemeral random constant generator primitive.
 */
Ephemeral::Ephemeral() :
  Primitive(0, "E")
{ }


/*!
 *  \brief Dummy function, ephemeral primitive is used only to generate constants.
 *  \param outDatum Result of the Divide operation.
 *  \param ioContext Evolutionary context.
 */
void Ephemeral::execute(void* outDatum, Context& ioContext)
{ }


/*!
 *  \brief Generate random constant and return as primitive handle.
 *  \param ioContext Evolutionary context.
 */
PrimitiveHandle Ephemeral::giveReference(Context& ioContext)
{
  double lValue = ioContext.mRandom.rollUniform(-1.0, 1.0);
  std::ostringstream lOSS;
  lOSS << lValue;
  return new TokenT<double>(lOSS.str(), lValue);
}
