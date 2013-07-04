#include "GrowTree.hpp"
#include "../System.hpp"
#include "../ADF.hpp"
#include "../ADFarg.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace robgp;
using namespace robgp::BreedOperators;

GrowTree::GrowTree(int minDepth, int maxDepth):mindepth(minDepth),maxdepth(maxDepth)
{
	name="GrowTree";
}

GrowTree::~GrowTree()
{

}

void growtree(System* system, int returntype, TreeNodeIter it, int depth)
{
	bool usefunctions=true;
	if (returntype>system->functions.size())
		throw runtime_error("Invalid return type, no functions for it");
	int i;
	if (depth<=0)
	{
		usefunctions=false;
	}

	// Choose a function given the type requirement
	int choices=0;
	for (i=0;i<system->functions[returntype].size();++i)
	{
		if (system->functions[returntype][i]->max_args() <=0)
			choices++;
		else if (usefunctions && system->functions[returntype][i]->min_args() >= 1 )
			choices++;
	}

	if (choices==0)
	{
		// Couldn't select desired type, choose random function
		i=rand()%system->functions[returntype].size();
	}else{
		int choice=rand()%choices;	// Choose a random function
		for (i=0;i<system->functions[returntype].size();++i)
		{
			if (system->functions[returntype][i]->max_args() <=0 && --choice < 0)
				break;
			else if (usefunctions && system->functions[returntype][i]->min_args() >= 1 && --choice < 0)
				break;
		}
	}

	if (i>=system->functions[returntype].size())
		throw runtime_error("Somehow a valid function was not selected");
	TreeNode newnode=system->functions[returntype][i]->createInstance();
	TreeNodeIter cnode=it.appendChild(newnode);

	for (i=0;i<newnode.arity;++i)
	{
		int atype;
		if (newnode.f->arg_type.size()>i+1)
			atype=newnode.f->arg_type[i+1];
		else
			atype=newnode.f->arg_type.back();
		growtree(system, atype, cnode, depth-1);
	}
}

void GrowTree::growTree(System* system, TreeNodeIter pos, int type)
{
	growtree(system, type, pos, (maxdepth<=mindepth?0:(rand()%(maxdepth-mindepth+1)))+mindepth);
}

int GrowTree::breed(System* system, Population* src, vector<Program>* newpop, int index)
{
	Program* newp;
	newpop->push_back(Program(system));
	newp=&newpop->back();

	newp->functions.assign(system->adf.begin(), system->adf.end());

	newp->branch.push_back(vector<TreeNode>());

	// Push the main program branch on
	growtree(system, system->returntype, TreeNodeIter(&newp->branch[0], 0), (maxdepth<=mindepth?0:(rand()%(maxdepth-mindepth+1)))+mindepth);

	// Need to add ADF branches
	for (int i=0;i<system->adf.size();++i)
		newp->branch.push_back(vector<TreeNode>());

	ADF* curfunc;
	for (int i=system->adf.size()-1;i>=0;--i)
	{
		curfunc = (ADF*)(system->adf[i]);

		// Remove current function from list
		if (system->recursiveDepth<=0)
		{
#ifdef DEBUG
			if (system->functions[curfunc->arg_type[0]].back()!=curfunc)
				throw runtime_error("Function being popped off of system does not match last adf entry");
#endif
			system->functions[curfunc->arg_type[0]].pop_back();
		}

		// Push function arguments
		for (int j=1;j<curfunc->arg_type.size();++j)
			system->functions[curfunc->arg_type[j]].push_back(curfunc->args[j-1]);

		// Generate tree
		growtree(system, curfunc->arg_type[0], TreeNodeIter(&newp->branch[i+1], 0), (maxdepth<=mindepth?0:(rand()%(maxdepth-mindepth+1)))+mindepth);

		// Pop function arguments
		for (int j=curfunc->arg_type.size()-1;j>=1;--j)
		{
			system->functions[curfunc->arg_type[j]].pop_back();
		}
	}

	if (system->recursiveDepth<=0)
	{
		for (int i=0;i<system->adf.size();++i)
		{
			curfunc = (ADF*)system->adf[i];
			system->functions[curfunc->arg_type[0]].push_back(curfunc);
		}
	}

#ifdef DEBUG
	if (!newp->checkConsistency())
	{
		throw runtime_error("Invalid produced program");
	}
#endif

	return 1;
}

BreedOperator* GrowTree::clone()
{
	GrowTree* newtree = new GrowTree();
	*newtree = *this;
	return newtree;
}

void GrowTree::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("mindepth")!="")
		mindepth=atoi(xml.get_attribute("mindepth").c_str());
	if (xml.get_attribute("maxdepth")!="")
		maxdepth=atoi(xml.get_attribute("maxdepth").c_str());
}

