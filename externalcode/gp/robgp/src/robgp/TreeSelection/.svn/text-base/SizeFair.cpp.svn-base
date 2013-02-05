#include "SizeFair.hpp"
#include "../System.hpp"
#include <stdexcept>
#include <cstdlib>
using namespace std;
using namespace robgp;
using namespace robgp::TreeSelection;

SizeFair::SizeFair()
{
	name="SizeFair";
}

SizeFair::~SizeFair()
{

}

TreeSelectionMethod* SizeFair::clone()
{
	TreeSelectionMethod* c = new SizeFair();
	*c=*this;
	return c;
}

int SizeFair::selectTree(int branch, Program* p, Program* prev, int prevsel, int type)
{

	if (prev==NULL || prevsel<0 || prevsel>=prev->branch[branch].size())
		throw runtime_error("Invalid previous tree selection");

	// Get the depth of the other tree, in order to select a subtree of roughly the same size
	int targetdepth=prev->branch[branch][prevsel].depth;

	// Remember the closest depth we've found to the target
	int founddepth=-1;
	vector<int> choices;

	int nodecount=0;
	int i;
	nodecount=0;
	for (i=0;i<p->branch[branch].size();++i)
	{
		if ((type<0 || p->branch[branch][i].f->arg_type[0]==type))
		{
			int depthoffset=abs(p->branch[branch][i].depth-targetdepth);
			if (founddepth==-1 || depthoffset<founddepth)
			{
				choices.clear();
				founddepth=depthoffset;
			}
			if (founddepth==depthoffset)
			{
				choices.push_back(i);
			}
		}
	}
	if (choices.empty())
		throw BreedError("No selection choices of same type");

	int selnode=rand()%choices.size();
	return choices[selnode];

}

