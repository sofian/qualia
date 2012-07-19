#pragma once
#include "Datatype.hpp"
#include "Frame.hpp"
#include <vector>
#include <string>

namespace robgp
{
	using namespace std;

// Forward reference
class TreeNode;
class Program;

/**
 * This class represents the base function object in the GP system. It has an
 * evaluator which may call subtree's evaluators whenever it wishes.
 */
class Function
{
public:
	Function();
	virtual ~Function();

	/**
	 * Evaluates this function given the list of arguments. These arguments
	 * will not currently be evaluated, which is why a helper evalAll(args)
	 * function exists in this class for cases where evaluation is not
	 * selective.
	 */
	virtual Datatype eval(TreeNodeIter curnode, Frame* frame)=0;

	/**
	 * The return type and types of each function argument. The first element
	 * is reserved for the return type of this function.
	 */
	vector<int> arg_type;

	/// Calculates and returns the maximum number of arguments for this function
	int max_args();

	/// Calculates and returns the minimum number of arguments for this function
	int min_args();

	/// The maximum number of arguments for this function
	int maxargs;

	/// The minimum number of arguments for this function
	int minargs;

	/**
	 * This function should output a string that ideally will run as
	 * independent C code. This is a step up from the pseudo-LISP format
	 * offered by most GP systems as a standard form of output.
	 */
	virtual void write(ostream& out, TreeNodeIter curnode);

	/**
	 * Create a usable node of a tree using this function.
	 */
	virtual TreeNode createInstance();

	/**
	 * The name of this function for text output purposes
	 */
	string name;
protected:

	/**
	 * This helper method evaluates all of the child nodes of the given
	 * tree and returns an array containing their return values.
	 *
	 * @param curnode	The node to process children of
	 * @param frame		The current execution frame. This includes
	 *			local argument values.
	 */
	vector<Datatype> evalAll(TreeNodeIter curnode, Frame* frame);

};

}
