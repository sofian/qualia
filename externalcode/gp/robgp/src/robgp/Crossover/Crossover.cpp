#include "Crossover.hpp"
#include "../TreeNode.hpp"
#include "../System.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace robgp;
using namespace robgp::BreedOperators;

Crossover::Crossover()
{
	name = "crossover";
	srcTrees = dstTrees = 2;
}

Crossover::~Crossover()
{

}

BreedOperator* Crossover::clone()
{
	Crossover* c = new Crossover();
	*c=*this;
	return c;
}

int Crossover::breed(System* system, Population* src, vector<Program>* newpop, int index)
{
	// Big complex mess
	// First select individuals
	vector<Program*> prgms=getSelections(2, src, index);

	Program* a = prgms[0];
	Program* b = prgms[1];

	// Subtree selection
	int branch=rand()%(a->branch.size());
	int i,j;
	vector<int> strees=getSelectionTrees(branch, &prgms);
	i=strees[0];
	j=strees[1];

	if (i<0 || j<0)
		return 0;

	// Detect early on if crossover will be unsuccessful due to depth limits
	if (system->maxdepth>0)
	{
		// Only need to check the new branch to see if it exceeds the limit
		int deptha=b->branch[branch][j].parentOffset;
		int depthb=a->branch[branch][i].parentOffset;
		int parent;
		parent=i;
		while (parent>0)
		{
			parent+=a->branch[branch][parent].parentOffset;
			deptha++;
		}
		parent=j;
		while (parent>0)
		{
			parent+=b->branch[branch][parent].parentOffset;
			depthb++;
		}
		if (deptha>system->maxdepth || depthb>system->maxdepth)
			return 0;
	}

	if (system->maxnodes>0)
	{
		// Check if either program will have too many nodes
		if (a->branch[branch][0].nodes-a->branch[branch][i].nodes+b->branch[branch][j].nodes>system->maxnodes)
			return 0;
		if (b->branch[branch][0].nodes-b->branch[branch][j].nodes+a->branch[branch][i].nodes>system->maxnodes)
			return 0;
	}

	// Swap subtrees rooted at a and b to form new nodes
	newpop->push_back(Program(system));
	newpop->push_back(Program(system));
	Program* dsta=&newpop->at(newpop->size()-2);
	Program* dstb=&newpop->at(newpop->size()-1);
	a->copy(*dsta);
	b->copy(*dstb);
	copyTree(TreeNodeIter(&dsta->branch[branch], i), TreeNodeIter(&b->branch[branch], j));
	copyTree(TreeNodeIter(&dstb->branch[branch], j), TreeNodeIter(&a->branch[branch], i));
#ifdef DEBUG
	if (!a->checkConsistency())
		throw runtime_error("Resulting program from architecture altering operation is not consistent");
	if (!b->checkConsistency())
		throw runtime_error("Resulting program from architecture altering operation is not consistent");

#endif
	return 2;
}



