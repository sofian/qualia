
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
 *  \file   puppy/Tree.hpp
 *  \brief  Definition of the class Tree.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3 $
 *  $Date: 2004/02/11 17:16:33 $
 */

#ifndef Puppy_Tree_hpp
#define Puppy_Tree_hpp

#include <iostream>
#include <vector>

#include "puppy/PrimitiveHandle.hpp"
#include "puppy/Primitive.hpp"
#include "puppy/Context.hpp"

namespace Puppy {


/*!
 *  \struct Node puppy/Tree.hpp "puppy/Tree.hpp"
 *  \brief GP tree node structure.
 *  \ingroup Puppy
 */
struct Node {

  /*!
   *  \brief Construct a tree node.
   *  \param inPrimitive Reference to the associated primitive.
   *  \param inSubTreeSize Sub-tree size value.
   */
  inline explicit Node(PrimitiveHandle inPrimitive=NULL, unsigned int inSubTreeSize=0) :
    mPrimitive(inPrimitive),
    mSubTreeSize(inSubTreeSize)
  { }
  
  PrimitiveHandle mPrimitive;    //!< Smart pointer to the associated primitive.
  unsigned int    mSubTreeSize;  //!< Sub-tree size, including actual node.
};


/*!
 *  \class Tree puppy/Tree.hpp "puppy/Tree.hpp"
 *  \brief GP tree class.
 *  \ingroup Puppy
 *
 *  The evolutionary context includes the execution context used when interpreting the trees
 *  along with the problem set-up defined with the function and terminal set, and the randomizer.
 */
class Tree : public std::vector<Node> {

public:

  explicit Tree(float inFitness=-1.0, bool inValid=false);

  inline bool operator==(const Tree& inRightTree) const;
  inline bool operator<(const Tree& inRightTree) const;
  
  unsigned int getDepth(unsigned int inIndex=0) const;
  void         interpret(void* outResult, Context& ioContext);
  void         setStackToNode(unsigned int inIndex, std::vector<unsigned int>& outCallStack) const;
  void         write(std::ostream& ioOS, unsigned int inIndex=0) const;

  float mFitness;  //!< Fitness value of the GP tree
  bool  mValid;    //!< Flag indicating whether the fitness is valid.
              
};

}

inline std::ostream& operator<<(std::ostream& ioOS, const Puppy::Tree& inTree);


/*!
 *  \brief Compare equality of two trees.
 *  \param inRightTree Second tree to compare to the actual.
 *  \return True is trees are equals, false if not.
 */
inline bool Puppy::Tree::operator==(const Puppy::Tree& inRightTree) const
{
  return (mValid && inRightTree.mValid && (mFitness == inRightTree.mFitness));
}


/*!
 *  \brief Compare ranking of two trees.
 *  \param inRightTree Second tree to compare to the actual.
 *  \return True is actual tree is less than seconf, false if not.
 */
inline bool Puppy::Tree::operator<(const Puppy::Tree& inRightTree) const
{
  return (mValid && inRightTree.mValid && (mFitness < inRightTree.mFitness));
}


/*!
 *  \brief Write tree into output stream with function Puppy::Tree::write.
 *  \param ioOS C++ output stream to write the tree into.
 *  \param inTree Tree to write into output stream.
 *  \return C++ output stream ioOS.
 *  \ingroup Puppy
 *  \relates Puppy::Tree
 */
inline std::ostream& operator<<(std::ostream& ioOS, const Puppy::Tree& inTree)
{
  inTree.write(ioOS);
  return ioOS;
}


#endif // Puppy_Tree_hpp
