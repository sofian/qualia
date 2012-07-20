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
 *  \file   ParityPrimits.cpp
 *  \brief  Implementation parity primitives.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/10 21:26:08 $
 */

 
#include "ParityPrimits.hpp"

using namespace Puppy;


/*!
 *  \brief Construct And GP primitive.
 */
And::And() :
  Primitive(2, "AND")
{ }


/*!
 *  \brief Execute characteristic operation of logical And primitive.
 *  \param outDatum Result of the logical And operation.
 *  \param ioContext Evolutionary context.
 */
void And::execute(void* outDatum, Context& ioContext)
{
  bool& lResult = *(bool*)outDatum;
  bool lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult = lResult && lArg2;
}


/*!
 *  \brief Construct Or GP primitive.
 */
Or::Or() :
  Primitive(2, "OR")
{ }


/*!
 *  \brief Execute characteristic operation of logical Or primitive.
 *  \param outDatum Result of the logical Or operation.
 *  \param ioContext Evolutionary context.
 */
void Or::execute(void* outDatum, Context& ioContext)
{
  bool& lResult = *(bool*)outDatum;
  bool lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult = lResult || lArg2;
}


/*!
 *  \brief Construct Xor GP primitive.
 */
Xor::Xor() :
  Primitive(2, "XOR")
{ }


/*!
 *  \brief Execute characteristic operation of logical Xor primitive.
 *  \param outDatum Result of the logical Xor operation.
 *  \param ioContext Evolutionary context.
 */
void Xor::execute(void* outDatum, Context& ioContext)
{
  bool& lResult = *(bool*)outDatum;
  bool lArg2;
  getArgument(0, &lResult, ioContext);
  getArgument(1, &lArg2, ioContext);
  lResult = (lResult != lArg2);
}


/*!
 *  \brief Construct Not GP primitive.
 */
Not::Not() :
  Primitive(1, "NOT")
{ }


/*!
 *  \brief Execute characteristic operation of Not primitive.
 *  \param outDatum Result of the Not operation.
 *  \param ioContext Evolutionary context.
 */
void Not::execute(void* outDatum, Context& ioContext)
{
  bool& lResult = *(bool*)outDatum;
  getArgument(0, &lResult, ioContext);
  lResult = (lResult == false);
}

