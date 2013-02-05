#include "BreedPhase.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <cstdlib>
using namespace std;
using namespace robgp;

BreedPhase::BreedPhase()
{
	totalrate=0;
}

BreedPhase::~BreedPhase()
{
	for (vector<BreedOperator*>::iterator it = breedops.begin(); it != breedops.end(); ++it)
		delete *it;
}

void BreedPhase::breed(System* system, Population* src, Population* dst)
{
	// First determine number of times each breed operator is to be used

	int timesleft=dst->psize;
	int* times = new int[breedops.size()];
	for (int i=0;i<breedops.size()-1;++i)
	{
		times[i]=dst->psize*breedops[i]->rate / totalrate;
		timesleft-=times[i];
	}
	times[breedops.size()-1]=timesleft;

	// Then produce new population
	vector<Program> gen;
	int selection;
	int breedind;
	timesleft = dst->psize;

	while (timesleft>0)
	{
		for (selection = rand() % timesleft,breedind=0;selection>=times[breedind];selection-=times[breedind],++breedind);
		try
		{
		int num=breedops[breedind]->breed(system, src, &gen, times[breedind]-1);
		for (int i=0;i<num;++i)
		{
			int ind=gen.size()-1-i;
			for (int br=0;br<gen[ind].branch.size();++br)
			{
				if ((system->maxnodes>0 && gen[ind].branch[br][0].nodes>system->maxnodes) || (system->maxdepth>0 && gen[ind].branch[br][0].depth>system->maxdepth))
				{
					// This individual exceeds limits and needs to be removed
					gen.erase(gen.begin()+ind);
					num--;
					i--;
					break;
				}
			}
		}
		while (num>times[breedind])
		{
			gen.pop_back();
			--num;
		}
		times[breedind]-=num;
		timesleft-=num;
		}catch (BreedError e)
		{
			// Aborting breed due to error
			//cerr << "Warning, error " << e.problem << endl;
		};
	}

	delete [] times;
	dst->programs.swap(gen);
}

void BreedPhase::parseXML(XmlReader& xml, System* system)
{
	if (xml.get_name()!="breed")
	{
		throw runtime_error("Expected breed tag, found "+xml.get_name()+" at line "+xml.get_line_no());
	}

	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		if (xml.get_name()!="#text")
		{
			// Search for breed operator in system's list of breed operators
			BreedOperator* op=system->getBreedOperator(xml.get_name());
			if (op==NULL)
			{
				throw runtime_error("RobGP system does not have a breed operator loaded called "+xml.get_name()+" referenced at line "+xml.get_line_no());
			}
			op=op->clone();
			op->parseXML(xml, system);
			breedops.push_back(op);
			totalrate+=op->rate;
		}
	}
}

void BreedPhase::updateRates()
{
	totalrate=0;
	for (vector<BreedOperator*>::iterator it=breedops.begin();it!=breedops.end();++it)
	{ 
		totalrate+=(*it)->rate;
	}
}

