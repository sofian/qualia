#pragma once
#include "ADF.hpp"
#include <vector>
#include <string>
#include "Selectable.hpp"
#include "Problem.hpp"

namespace robgp
{
using namespace std;

class System;

/**
 * This class stores the complete representation for a single program. A
 * program consists of one or more functions, the first of which is called
 * to execute the program. This main function may call the other functions
 * which are automatically created / deleted and modified (ADF's with
 * architecture altering operations).
 */

class Program : public Selectable
{
public:
	/**
	 * Construct a new empty program
	 *
	 * @param system	The system to which this program belongs
	 */
	Program(System* system);

	~Program();

#ifdef DEBUG
	/**
	 * Check the consistency of the program to ensure that no corruption of
	 * its internal nodes has occurred. This is used regularly when the GP
	 * is compiled with the DEBUG flag so that you can know as soon as an
	 * error occurs.
	 */
	bool checkConsistency();

	/**
	 * Convert the current program into a string representation.
	 */
	string to_string();
#endif

	/**
	 * Evaluates the program on the given problem and problem instance. The
	 * problem instance is kept separate from the problem so as to allow
	 * re-use of the same read-only problem on multithreaded operations.
	 *
	 * @param prob	The problem data, should be used read-only
	 * @param inst	The problem instance containing state information
	 */
	Datatype eval(Problem* prob=NULL, ProblemInstance* inst=NULL);

	/**
	 * Copy the given program including all subtrees to the target program
	 * specified.
	 *
	 * @param result	The program to be overwritten by this one.
	 */
	void copy(Program& result);

	/**
	 * Function declarations for each program branch
	 */
	vector<Function*> functions;

	/// The system to which this program belongs
	System* system;

	/**
	 * Store the functions in a preorder tree in a vector. This will
	 * minimize memory usage and memory allocations.
	 */
	vector< vector<TreeNode> > branch;

	/**
	 * The less than operator will be used for ordering programs from
	 * best to worst so that the program may easily fetch the top X
	 * programs.
	 *
	 * @param other	The program to compare fitness against
	 * @return	True if this program is better than the other supplied program
	 */
	bool operator< (const Program& other) const;

};

};

/**
 * Write the program to the given output stream
 *
 * @param os	The output stream to write the plaintext program to
 * @param prgm	The program to write to the output stream
 */
std::ostream& operator<< (std::ostream& os, robgp::Program& prgm);

