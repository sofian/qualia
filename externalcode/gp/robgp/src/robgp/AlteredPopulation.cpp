#include "AlteredPopulation.hpp"
using namespace std;
using namespace robgp;
#include "System.hpp"
#include <stdexcept>

AlteredPopulation::AlteredPopulation(int size):Population(size)
{
	preparedbr=-1;
}

AlteredPopulation::AlteredPopulation(Population* subpop, int groups):Population(subpop, groups)
{
	preparedbr=-1;
	functions.assign(groups, vector<Function*>());
}

AlteredPopulation::~AlteredPopulation()
{
	for (vector<vector<Function*> >::iterator it = functions.begin(); it!=functions.end(); ++it)
	{
		for (vector<Function*>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
			delete *it2;
	}
}

void AlteredPopulation::setSize(int psize)
{
	Population::setSize(psize);
//	cerr << "New size is "<<this->psize<<endl;
/*	functions.clear();
	while (functions.size()<subpops.size())
		functions.push_back(vector<Function*>());*/
}

void AlteredPopulation::copy(Population* dst)
{
	// Copy the programs
	Population::copy(dst);

	// Then copy the function list, first the functions in the old population must now be removed
	AlteredPopulation* dstp=dynamic_cast<AlteredPopulation*>(dst);
	if (dstp==NULL)
		throw runtime_error("For some reason trying to copy an altered architecture to a regular population");

	// Delete all old functions
	for (int i=0;i<dstp->functions.size(); ++i)
	{
		for (int j=0;j<dstp->functions[i].size(); ++j)
		{
			delete dstp->functions[i][j];
		}
		dstp->functions[i].clear();
	}
	dstp->functions.clear();

	// Now copy the other population's functions
	for (int i=0;i<functions.size(); ++i)
	{
		dstp->functions.push_back(vector<Function*>());
		for (int j=0;j<dstp->functions[i].size(); ++j)
			dstp->functions[i].push_back(((ADF*)functions[i][j])->clone());
	}

	// Now search for calls to old function .. in each program in the dest pop
	// Wait, this shouldn't be done, for now we won't support nesting of altered populations
	throw runtime_error("Nesting of altered populations is not yet supported - sorry");

	// In order to support nesting of altered populations, complete the following code to change
	// references of the new population's adf calls to correspond to the new adf function objects
	/*
	for (int pi=0;pi<dstp->selSize();++pi)
	{
		Program* prgm = (Program*)(dstp->selAt(pi));
		// Operate
		for (int br=0;br<prgm->branch.size();++br)
		{
			for (int node=0;node<prgm->branch[br].size();++node)
			{
				if (typeid(*(prgm->branch[br][node].f))==typeid(ADF))
				{
					prgm->branch[br][node].f=dstp->functions[
				}
			}
		}
	}*/
}

void AlteredPopulation::prepareADFs(System* system, int subpop, int branchno)
{
	if (preparedbr>=0)
		clearADFs(system, subpop);
	preparedbr = branchno;
	int adfno;
	if (branchno==0)
		adfno=functions[subpop].size();
	else
		adfno=branchno - system->adfs - 1;

	for (int i=0;i < adfno; ++i)
	{
		// For each adf, push its function entry onto the callable functions
		Function* newfunc = functions[subpop][i];

		system->functions[newfunc->arg_type[0]].push_back(newfunc);	// Push function entry onto callable functions
		system->adf.push_back(newfunc);
	}

	// Then push function arguments
	if (branchno > 0)
	{
		ADF* newfunc = (ADF*)functions[subpop][branchno - system->adfs - 1];
		for (int i=0;i<newfunc->args.size();++i)
			system->functions[newfunc->arg_type[i+1]].push_back(newfunc->args[i]);
	}
}

void AlteredPopulation::clearADFs(System* system, int subpop)
{
	if (preparedbr>=0)
	{
		// Remove adf arguments
		if (preparedbr>0)
		{
			ADF* addedfunc = (ADF*)functions[subpop][preparedbr - system->adfs - 1];
			for (int i=0;i<addedfunc->args.size();++i)
				system->functions[addedfunc->arg_type[i+1]].pop_back();
		}
		
		// Then remove adf branches
		int adfno;
		if (preparedbr==0)
			adfno=functions[subpop].size();
		else
			adfno=preparedbr - system->adfs - 1;

		for (--adfno;adfno>0;--adfno)
		{
			Function* newfunc = functions[subpop][adfno];
			system->adf.pop_back();

			system->functions[newfunc->arg_type[0]].pop_back();	// Pop function entry off callable functions	
		}
		
	}
	preparedbr = -1;
}

Population* AlteredPopulation::clone()
{
	AlteredPopulation* clone=new AlteredPopulation(psize);
	for (int i=0;i<subpops.size();++i)
		clone->subpops.push_back(subpops[i]->clone());
	return clone;
}


