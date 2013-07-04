#pragma once
#include <stack>
#include <vector>
#include "Datatype.hpp"
#include "TreeNode.hpp"
#include "Problem.hpp"
namespace robgp
{
using namespace std;

class Program;

/**
 * During execution of a normal program, there is a stack frame that gives
 * information on local variables. During execution of a GP program, the system
 * uses the same idea to keep track of local arguments and callable ADF's.
 */

class Frame
{
public:
	/**
	 * Constructs a new frame for the supplied program, to use in
	 * execution.
	 *
	 * @param prgm	The program to prepare an execution frame for.
	 */
	Frame(Program* prgm, int maxdepth);
	~Frame();

	/**
	 * Push a new set of arguments onto the execution frame
	 *
	 * @param newargs	The new argument list in execution
	 */
	void push(vector<Datatype>& newargs);

	/**
	 * Pop the last function's loaded arguments from the frame
	 */
	void pop();

	/// The problem being worked on
	Problem* prob;

	/// The current modifiable instance of the problem
	ProblemInstance* inst;

	/// The list of automatically defined functions available for calling
	vector<TreeNodeIter> adfs;

	/// The list of local arguments currently
	vector<Datatype>* args;

	/// The current frame depth
	int depth;

	/// The maximum frame depth
	int maxdepth;
private:
	/// The complete stack of argument values
	stack<vector<Datatype> > f_args;
};

}

