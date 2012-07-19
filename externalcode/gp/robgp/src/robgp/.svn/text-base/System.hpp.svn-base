#pragma once
#include <map>
#include <vector>
#include "Function.hpp"
#include "Population.hpp"
#include "BreedOperator.hpp"
#include "BreedPhase.hpp"
#include "BreedPopOperator.hpp"
#include "Evaluator.hpp"
#include "Problem.hpp"
#include "PopOperations/Coevolution.hpp"
#include <stdexcept>
namespace robgp
{
	using namespace std;

	/**
	 * This class is a general exception class for modules of the GP to let
	 * the system know that an error occurred. Generally the response is to
	 * retry with a new operation.
	 */
	class BreedError
	{
	public:
		/// The identified problem, useful for debugging
		string problem;

		/**
		 * Construct a breed error with a given problem description
		 *
		 * @param problem	The problem that occurred
		 */
		BreedError(string problem)
		{
			this->problem=problem;
		}

	};

/**
 * The system class will oversee the operations of the GP. It will be to this class
 * that the function / terminal lists must be added. All GP operators are loaded
 * into this class before parsing the input XML file.
 */

class System
{
public:
	System();
	~System();

	/**
	 * Process the named breed phase. This will run through one generation
	 * of breeding using the breed phase read in from the input XML file.
	 *
	 * @param pname	The name of the breed phase from the XML file
	 */
	void ProcBreedPhase(string pname);

	/**
	 * Process a population breeding phase. This will run through one
	 * generation of a population breed phase from the input XML file.
	 *
	 * @param pname	The name of the population breed phase from the XML file
	 */
	void ProcBreedPop(string pname);

	/**
	 * The stored list of functions. These are stored by return type for quick
	 * access to functions returning a certain type.
	 */
	vector<vector<Function*> > functions;

	/**
	 * Gets the type integer of a named type. This will create a new stored
	 * type if the requested type does not yet exist.
	 *
	 * @param type	The name of the type to get
	 * @return	The index of the requested type
	 */
	int gettype(string type);

	/**
	 * Gets the name of the given integer type. This is used internally to
	 * produce pretty output using the user supplied type strings
	 *
	 * @param typeno	The index of the type to get the name for
	 * @return		The name of the type
	 */
	string typestr(int typeno);

	/**
	 * Sets a problem source to get problems for solving from. Using a
	 * problem source lets you take advantage of automated fitness
	 * weighting and other such features.
	 *
	 * @param source	The problem source to get problem instances from
	 */
	void setProblemSource(ProblemSource* source);

	/**
	 * Parse the given command-line arguments. These may override options in
	 * the XML file such that different configurations can be tried without
	 * creating new configuration files.
	 */
	void parseArgs(int argc, char** argv);

	/**
	 * Parse the given input stream for the XML representation of the GP
	 * configuration to use.
	 *
	 * @param input	The input stream to read the XML GP configuration from
	 */
	void parseStream(istream* input);

	/**
	 * Opens the given file and passes it to the parseStream method to read
	 * XML data from.
	 *
	 * @param filename	The name of the file to open and read XML data from.
	 */
	void parseFile(string filename);

	/**
	 * Add a function for the GP system to use.
	 *
	 * @param func	The function to add to the system
	 */
	bool addFunction(Function* func);

	/**
	 * Get the breed operator given by the supplied name
	 *
	 * @param name	The name to look up
	 */
	BreedOperator* getBreedOperator(string name);

	/**
	 * Register a new breed operator for use by the system.
	 *
	 * @param breedop	The new operator to use
	 * @return		True if no operator by this name already existed
	 */
	bool registerBreedOperator(BreedOperator* breedop);

	/**
	 * Get the population breeding  operator given by the supplied name
	 *
	 * @param name	The name to look up
	 */
	BreedPopOperator* getBreedPopOperator(string name);

	/**
	 * Register a new population breeding operator for use by the system.
	 *
	 * @param breedop	The new operator to use
	 * @return		True if no operator by this name already existed
	 */
	bool registerBreedPopOperator(BreedPopOperator* breedop);

	/**
	 * Get the selection method given by the supplied name
	 *
	 * @param name	The name to look up
	 */
	SelectionMethod* getSelectionMethod(string name);

	/**
	 * Register a new selection method for use by the system.
	 *
	 * @param breedop	The new operator to use
	 * @return		True if no operator by this name already existed
	 */
	bool registerSelectionMethod(SelectionMethod* breedop);

	/**
	 * Get the tree selection method given by the supplied name
	 *
	 * @param name	The name to look up
	 */
	TreeSelectionMethod* getTreeSelectionMethod(string name);

	/**
	 * Register a new tree selection method for use by the system.
	 *
	 * @param breedop	The new operator to use
	 * @return		True if no operator by this name already existed
	 */
	bool registerTreeSelectionMethod(TreeSelectionMethod* breedop);

	/// Current population size
	int popsize;

	/// Expected return type from main
	int returntype;

	/// The number of objectives
	int objectives;

	/// The weight of each objective. Used differently depending on multi-objective mode
	std::vector<double> objectiveWeight;

	/// If non-zero, recursive ADF's are allowed and will execute up to this depth
	int recursiveDepth;

	/**
	 * The breed phases set up for this system
	 */
	map<string, BreedPhase> breed;
	
	/**
	 * Population breeding operators set up
	 */
	map<string, BreedPopOperator*> breedpop;

	/**
	 * The evaluator currently in use
	 */
	Evaluator* eval;

	/**
	 * Coevolution evaluator
	 */
	CoevolutionEvaluator* coeval;

	/// The coevolution population breeding phase in use
	PopOperations::Coevolution* coev_phase;

	/// The population of individuals
	Population* pop;

	/// The number of ADF's hardcoded into this population
	int adfs;
	/// The list of ADF's hardcoded into the population
	vector<Function*> adf;

	/**
	 * GP-wide limit on the maximum depth of a program. If this is exceeded,
	 * a breed operator may fail. Negative values mean no limit.
	 */
	int maxdepth;

	/**
	 * GP-wide limit on the maximum number of nodes in a program tree. If this
	 * is exceeded, a breed operator may fail. Negative values mean no limit.
	 */
	int maxnodes;

	/**
	 * Evaluate the given group of programs using the coevolution evaluator
	 *
	 * @param prgms	The list of programs to evaluate
	 */
	void coev_evaluate(vector<Program*> &prgms);

	/**
	 * Evaluate all of the loaded individuals
	 */
	void evaluateAll();

	/// If true, will compute ranks using pareto ranking
	enum rankMethod {WEIGHTED, PARETO, RANKEDSUM};
	
	rankMethod rankingMethod;

	/**
	 * If true, the rank of each objective is it's "position" within the
	 * population. Which means if you have 3 individuals with fitnesses
	 * 3 3 7 their resulting ranks will be 0 0 2.
	 */
	bool rankPosition;

	/**
	 * If true, divide by the maximum rank in each objective when adding
	 * it into the ranked sum.
	 */
	bool rankNormalize;

	int rankDiversityMultiplier;


private:

	/**
	 * Process the given breedphase on the given population. This will call
	 * itself recursively for subpopulations.
	 *
	 * @param p	The population to breed
	 * @param b	The breed phase to use on the population
	 */
	void BreedPhasePop(Population* p, BreedPhase* b);

	/**
	 * Evaluate the given program using the current evaluation scheme.
	 *
	 * @param prgm	The program to evaluate
	 */
	void evaluate(Program* prgm);

	/**
	 * Compute the ranks of the programs in the population
	 */
	void computeRanks();

	/**
	 * This map creates an association between an integer value and a type name.
	 * This way the user can identify types by name and have it converted to
	 * a value
	 */
	map<string, int> types;

	/**
	 * This stores the reverse association of type index to name
	 */
	vector<string> typestrs;

	/**
	 * This stores a list of loaded breed operators. Each one is an
	 * instantiated instance of its class, with a clone method to copy
	 * it and reuse it.
	 */
	map<string, BreedOperator*> breedops;

	/**
	 * This stores a list of loaded pop breed operators. Each one is an
	 * instantiated instance of its class, with a clone method to copy
	 * it and reuse it.
	 */
	map<string, BreedPopOperator*> breedpopops;

	/**
	 * Stores the list of selection operators available
	 */
	map<string, SelectionMethod*> selectionops;

	/**
	 * Stores the list of tree selection operators available
	 */
	map<string, TreeSelectionMethod*> treeselectionops;

	/**
	 * Provides test cases or problems for the GP to solve
	 */
	ProblemSource* problems;

	/**
	 * Maintenance operations relating to updating the problemset will occur here
	 */
	void updateProblemSet();

	/**
	 * Use weighted fitness if weighted > 0. The weight is what weighted is set to.
	 */
	double weighted;

	/**
	 * The target number of problems to load at one time. A value of -1 will
	 * load as many as are available.
	 */
	int problemSetSize;

	/// The number of generations between changing the loaded problem set
	int problemUpdateInterval;

	/// The number of generations remaining until the next problem set change
	int problemUpdateCountdown;

	/**
	 * This is a structure for storing statistics on loaded problems for
	 * processing. By keeping track of the average fitness, problems can be
	 * weighted according to how well the population does on them.
	 */
	typedef struct probstats
	{
		Problem* prob;
		vector<double> avg_fitness[2];
		int last;	// Which run of 0 / 1 was the last one

		probstats(System* s, Problem* p)
		{
			prob=p;
			last=0;
			avg_fitness[0].assign(s->objectives+2, 0.0);
			avg_fitness[1].assign(s->objectives+2, 0.0);
		}
	} probstats;

	/// The currently loaded problems for running the GP on
	vector<probstats> curprobs;
};

}

