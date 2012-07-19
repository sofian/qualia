#include "Mutation.hpp"
#include "../TreeNode.hpp"
#include "../System.hpp"
#include "../Utility.hpp"
#include <cstdlib>
#include <stdexcept>

using namespace std;
using namespace robgp;
using namespace robgp::BreedOperators;

Mutation::Mutation()
{
	grower = NULL;
	name = "mutation";
	srcTrees = dstTrees = 1;
}

Mutation::~Mutation()
{
	if (grower)
		delete grower;
}

BreedOperator* Mutation::clone()
{
	Mutation* c = new Mutation();
	*c=*this;
	return c;
}

int Mutation::breed(System* system, Population* src, vector<Program>* newpop, int index)
{
	vector<Program*> p = getSelections(1, src, index);
	int branch=rand()%p[0]->branch.size();
	vector<int> subtree = getSelectionTrees(branch, &p);

	newpop->push_back(Program(system));
	Program* newprog=&newpop->back();
	p[0]->copy(*newprog);

	ADF* curfunc=NULL;
	if (branch>0)
	{
		// Remove adf's greater than or equal to branch
		if (system->recursiveDepth<=0)
		{
			for (int i=system->adf.size()-1;i>=branch-1;--i)
			{
				// Remove current function from list
				curfunc = (ADF*)(system->adf[i]);
				system->functions[curfunc->arg_type[0]].pop_back();		
			}
		}
		else
			curfunc = (ADF*)(system->adf[branch-1]);

		// Push function arguments
		for (int j=1;j<curfunc->arg_type.size();++j)
			system->functions[curfunc->arg_type[j]].push_back(curfunc->args[j-1]);
	}

	// Generate tree
	if (grower==NULL)
		throw runtime_error("No tree grower for mutation defined");
	vector<TreeNode> newtree;
	grower->growTree(system, TreeNodeIter(&newtree, 0), newprog->branch[branch][subtree[0]].f->arg_type[0]);
	copyTree(TreeNodeIter(&newprog->branch[branch], subtree[0]), TreeNodeIter(&newtree, 0));

	if (branch>0)
	{
		// Pop function arguments
		for (int j=curfunc->arg_type.size()-1;j>=1;--j)
		{
			system->functions[curfunc->arg_type[j]].pop_back();
		}	

		// Push adf's
		if (system->recursiveDepth<=0)
		{
			for (int i=branch-1;i<system->adf.size();++i)
			{
				curfunc = (ADF*)system->adf[i];
				system->functions[curfunc->arg_type[0]].push_back(curfunc);
			}
		}
	}
	return 1;
}

void Mutation::parseXMLAttribute(XmlReader& xml, System* system)
{
	if (xml.get_name()=="grow")
	{
		for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
		{
			if (xml.get_name()!="#text")
			{
				// Search for breed operator in system's list of breed operators
				BreedOperator* op=system->getBreedOperator(xml.get_name());
				if (op==NULL)
				{
					throw runtime_error("RobGP system does not have a breed operator loaded called "+xml.get_name()+" referenced at line "+to_string(xml.get_line_no()));
				}
				op=op->clone();
				op->parseXML(xml, system);
				if (grower==NULL)
				{
					grower=dynamic_cast<TreeGrower*>(op);
					if (grower==NULL)
					{
						throw runtime_error("Passed operator "+(op->name)+" is not a tree growing operator");
						delete op;
					}
				}else{
						cerr << "Warning, extra grow operator not being used" << endl;
						delete op;
				}

			}
		}
	}else
		BreedOperator::parseXMLAttribute(xml, system);
}


