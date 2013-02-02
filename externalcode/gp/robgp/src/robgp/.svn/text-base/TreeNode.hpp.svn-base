#pragma once
#include "Datatype.hpp"
#include <vector>

namespace robgp
{
using namespace std;

// Forward reference
class System;
class Function;
class Program;
class Frame;

class TreeNodeIter;

class TreeNode
{
public:
	TreeNode(Function* func);
	~TreeNode();

	/**
	 * Pointer to the function this tree node uses for evaluation
	 */
	Function* f;

	/**
	 * Uses the GP's internal data type store one of the following:
	 *
	 * -Store the ADF number for ADF function calls. While this is not required
	 * for most nodes, it is more efficient to have it for all nodes than to
	 * have to dynamically allocate tree nodes to allow for polymorphism.
	 *
	 * -The ADF argument number for ADF argument functions
	 *
	 * -The value stored at this node for ERC nodes
	 */
	Datatype extra;

	/**
	 * Stores the branching value of this node
	 */
	int arity;

	/**
	 * The number of nodes in this subtree. This number can be used to easily skip this tree.
	 */
	int nodes;

	/**
	 * The depth of this subtree -- mainly used for depth-based selection operators
	 */
	int depth;

	/**
	 * The offset of this node's parent relative to the child. Changes in program tree structure may affect this.
	 */
	int parentOffset;

};

class TreeNodeIter
{
public:
	TreeNodeIter(vector<TreeNode>* t, int n);
	~TreeNodeIter();

#ifdef DEBUG
	bool checkConsistency(System* system, vector<Function*>* functions, int branch);
#endif

	TreeNodeIter parentNode();
	TreeNodeIter appendChild(TreeNode childnode);
	bool operator!= (TreeNodeIter other);
	TreeNode& operator* ();
	TreeNode* operator-> ();
	TreeNodeIter& operator++();

	TreeNodeIter begin();
	TreeNodeIter end();
	vector<TreeNode>::iterator iter();

	vector<TreeNode>* tree;
	int node;
};

	// Define several useful functions for working with treenodes
	void copyTree(TreeNodeIter dst, TreeNodeIter src, int paroffset=0);
	void copyTree(TreeNodeIter dst, TreeNode t);
	void eraseTree(TreeNodeIter t);

};

std::ostream& operator<< (std::ostream& os, robgp::TreeNodeIter& prgm);


