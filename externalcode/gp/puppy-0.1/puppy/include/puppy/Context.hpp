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
 *  \file   puppy/Context.hpp
 *  \brief  Definition of the class Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2 $
 *  $Date: 2004/02/11 15:46:20 $
 */

#ifndef Puppy_Context_hpp
#define Puppy_Context_hpp

#include <map>
#include <string>
#include <vector>

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/Randomizer.hpp"

namespace Puppy {

class Tree;  // Forward declaration


/*!
 *  \class Context puppy/Context.hpp "puppy/Context.hpp"
 *  \brief Evolutionary context.
 *  \ingroup Puppy
 *
 *  The evolutionary context includes the execution context used when interpreting the trees
 *  along with the problem set-up defined with the function and terminal set, and the randomizer.
 */
class Context {

public:

  /*!
   *  \brief Build an evolutionary context.
   */
  inline Context() :
    mTree(NULL)
  { }


  /*!
   *  \brief Add a new primitive in the sets of primitive.
   *  \param inPrimitive Primitive added.
   */
  inline void insert(PrimitiveHandle inPrimitive)
  {
    assert(mPrimitiveMap.find(inPrimitive->getName()) == mPrimitiveMap.end());
    mPrimitiveMap[inPrimitive->getName()] = inPrimitive;
    if(inPrimitive->getNumberArguments() == 0) mTerminalSet.push_back(inPrimitive);
    else mFunctionSet.push_back(inPrimitive);
  }
  
  Randomizer                            mRandom;        //!< Random number generator.
  std::vector<PrimitiveHandle>          mFunctionSet;   //!< Set of functions usable to build trees.
  std::vector<PrimitiveHandle>          mTerminalSet;   //!< Set of terminals usable to build trees.
  std::map<std::string,PrimitiveHandle> mPrimitiveMap;  //!< Name-primitive map.
  std::vector<unsigned int>             mCallStack;     //!< Execution call stack.
  Tree*                                 mTree;          //!< Actual tree evaluated.
      
};

}


#endif // Puppy_Context_hpp
