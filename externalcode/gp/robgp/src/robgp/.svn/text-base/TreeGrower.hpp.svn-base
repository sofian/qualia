#pragma once
#include "TreeNode.hpp"
#include "Function.hpp"
#include "BreedOperator.hpp"

namespace robgp
{

/**
 * This is an interface for breed operations that are capable of growing trees.
 * By implementing this interface a tree growing breed operation can be used by
 * operators that need to grow trees. For example mutation can be given any tree
 * grower.
 */

class TreeGrower : public BreedOperator
{
public:
	TreeGrower();
	virtual ~TreeGrower();

	/**
	 * This is the main method that a tree grower must define. The means by
	 * which a tree is constructed. It will be provided a function set, a
	 * return type for the tree root, and a maximum depth if applicable. A
	 * depth of -1 means unrestricted.
	 *
	 * @param system	System object
	 * @param pos		The position in the tree to replace
	 * @param type		The required return type for this grown tree
	 */
	virtual void growTree(System* system, TreeNodeIter pos, int type)=0;


};

};

