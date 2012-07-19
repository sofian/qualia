#pragma once
#include "Function.hpp"
#include "Frame.hpp"
#include "ADFarg.hpp"

namespace robgp
{

/**
 * This class represents the definition for an automatically defined function in
 * the function list.
 */

class ADF : public Function
{
public:
	ADF(vector<int> &argtypes, string name="", int adf_no=-1);
	virtual ~ADF();
	
	/**
	 * Evaluates the ADF by evaluating the subprogram
	 */
	virtual Datatype eval(TreeNodeIter curnode, Frame* frame);
	virtual void write(ostream& out, TreeNodeIter curnode);

	virtual TreeNode createInstance();

	/**
	 * This method gives the ADF an auto-generated name based on its
	 * index in the function list. This should be called after changing
	 * the function list during a run, as with architecture altering
	 * operations.
	 */
	virtual void rename();
	virtual Function* clone();

	/// The index of this ADF in the function list
	int adf_no;
	
	/// The argument types for this ADF
	vector<Function*> args;

private:

};

}

