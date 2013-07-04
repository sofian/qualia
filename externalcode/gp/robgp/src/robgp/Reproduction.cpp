#include "Reproduction.hpp"
#include "TreeNode.hpp"
#include "System.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace robgp;
using namespace robgp::BreedOperators;

Reproduction::Reproduction()
{
	name = "reproduction";
	srcTrees = dstTrees = 1;
}

Reproduction::~Reproduction()
{

}

BreedOperator* Reproduction::clone()
{
	Reproduction* c = new Reproduction();
	*c=*this;
	return c;
}

int Reproduction::breed(System* system, Population* src, vector<Program>* newpop, int index)
{
	// Big complex mess
	// First select individuals
	vector<Program*> p = getSelections(1, src, index);

	Program* a = p[0];

	// Swap subtrees rooted at a and b to form new nodes
	newpop->push_back(Program(system));
	Program* dsta=&newpop->at(newpop->size()-1);
	a->copy(*dsta);
#ifdef DEBUG
	if (!a->checkConsistency())
		throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
	return 1;
}


