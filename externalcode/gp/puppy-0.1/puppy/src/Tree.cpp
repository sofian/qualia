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
 *  \file   src/Tree.cpp
 *  \brief  Implementation of GP Tree methods.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3 $
 *  $Date: 2004/02/11 19:49:23 $
 */

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/PrimitiveInline.hpp"
#include "puppy/Randomizer.hpp"
#include "puppy/Context.hpp"
#include "puppy/Tree.hpp"
#include "puppy/TokenT.hpp"


/*!
 *  \brief Construct a new tree, with given fitness and validity flag.
 *  \param inFitness Fitness value of the GP tree.
 *  \param inValid Validity of the fitness value.
 */
Puppy::Tree::Tree(float inFitness, bool inValid) :
  mFitness(inFitness),
  mValid(inValid)
{ }


/*!
 *  \brief Return tree depth at given index.
 *  \param inIndex Index of sub-tree root to get the depth from.
 *  \return Sub-tree depth.
 */
unsigned int Puppy::Tree::getDepth(unsigned int inIndex) const
{
  assert(inIndex < size());
  unsigned int lNbArgs = (*this)[inIndex].mPrimitive->getNumberArguments();
  unsigned int lDepth = 1;
  unsigned int j = inIndex + 1;
  for(unsigned int i=0; i<lNbArgs; ++i) {
    unsigned int lChildDepth = getDepth(j) + 1;
    if(lChildDepth > lDepth) lDepth = lChildDepth;
    j += (*this)[j].mSubTreeSize;
  }
  return lDepth;
}


/*!
 *  \brief Interpret the GP tree.
 *  \param outResult Datum containing the result of the interpretation.
 *  \param ioContext Evolutionary context.
 */
void Puppy::Tree::interpret(void* outResult, Puppy::Context& ioContext)
{
  assert(size() > 0);
  ioContext.mTree = this;
  ioContext.mCallStack.push_back(0);
  front().mPrimitive->execute(outResult, ioContext);
  ioContext.mCallStack.pop_back();
}


/*!
 *  \brief Set call stack to include the correctly refer to a given node.
 *  \param inIndex Node index to which call stack must be set.
 *  \param outCallStack Result of call stack setting.
 */
void Puppy::Tree::setStackToNode(unsigned int inIndex,
                                 std::vector<unsigned int>& outCallStack) const
{
  assert(inIndex < size());
  outCallStack.resize(0);
  unsigned int i = 0;
  outCallStack.push_back(i);
  while(i < inIndex) {
    unsigned int lNbArgs=(*this)[i].mPrimitive->getNumberArguments();
    unsigned int lChildIndex = i + 1;
    for(unsigned int j=0; j<lNbArgs; ++j) {
      if((lChildIndex+(*this)[lChildIndex].mSubTreeSize) > inIndex) break;
      lChildIndex += (*this)[lChildIndex].mSubTreeSize;
    }
    assert(lChildIndex < size());
    i = lChildIndex;
    outCallStack.push_back(i);
  }
  assert(i == inIndex);
}


/*!
 *  \brief Write GP tree at given index as a s-expression into a C++ output stream.
 *  \param ioOS C++ output stream to write tree into.
 *  \param inIndex Actual node index in tree.
 */
void Puppy::Tree::write(std::ostream& ioOS, unsigned int inIndex) const
{
  assert(inIndex < size());
  unsigned int lNbArgs = (*this)[inIndex].mPrimitive->getNumberArguments();
  if(lNbArgs > 0) ioOS << '(';
  ioOS << (*this)[inIndex].mPrimitive->getName();
  unsigned int j = inIndex + 1;
  for(unsigned int i=0; i<lNbArgs; ++i) {
    ioOS << ' ';
    write(ioOS, j);
    j += (*this)[j].mSubTreeSize;
  }
  if(lNbArgs > 0) ioOS << ')';
}
