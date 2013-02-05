#include "System.hpp"
#include "AlteredPopulation.hpp"
using namespace std;
using namespace robgp;
#include <fstream>
#include "XmlReader.hpp"
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <cmath>

#include "Utility.hpp"
#include "Selectable.hpp"
#include "TreeGrower/FullTree.hpp"
#include "TreeGrower/GrowTree.hpp"
#include "Crossover/Crossover.hpp"
#include "Mutation/Mutation.hpp"
#include "Reproduction.hpp"
#include "Selection/TournamentSelection.hpp"
#include "Selection/RouletteWheel.hpp"
#include "Selection/Best.hpp"
#include "PopOperations/AlterArchitecture.hpp"
#include "PopOperations/Coevolution.hpp"
#include "PopOperations/IslandGP.hpp"
#include "PopOperations/ConfidenceTest.hpp"
#include "TreeSelection/SizeFair.hpp"

#define DOUBLE_PROX 0.000001

System::System()
{
	registerBreedOperator(new BreedOperators::FullTree());
	registerBreedOperator(new BreedOperators::GrowTree());
	registerBreedOperator(new BreedOperators::Crossover());
	registerBreedOperator(new BreedOperators::Mutation());
	registerBreedOperator(new BreedOperators::Reproduction());

	registerBreedPopOperator(new PopOperations::AlterArchitecture());
	registerBreedPopOperator(new PopOperations::Coevolution());
	registerBreedPopOperator(new PopOperations::IslandGP());
	registerBreedPopOperator(new PopOperations::ConfidenceTest());

	registerSelectionMethod(new Selection::TournamentSelection());
	registerSelectionMethod(new Selection::RouletteWheel());
	registerSelectionMethod(new Selection::Best());

	registerTreeSelectionMethod(new TreeSelection::SizeFair());

	pop = new Population(0);
	rankDiversityMultiplier=0;
	objectives=0;
	adfs=0;
	weighted=0;
	recursiveDepth=0;
	maxdepth=maxnodes=-1;
	rankPosition=true;
	rankNormalize=false;
	eval = NULL;
	coeval = NULL;
	coev_phase=NULL;
	problems = NULL;
	problemUpdateInterval=20;
	problemUpdateCountdown=0;
	problemSetSize=-1;
	rankingMethod=WEIGHTED;
}

System::~System()
{
	for (map<string, BreedOperator*>::iterator it = breedops.begin(); it != breedops.end(); ++it)
	{
		delete it->second;
	}

	for (map<string, BreedPopOperator*>::iterator it = breedpopops.begin(); it != breedpopops.end(); ++it)
	{
		delete it->second;
	}

	for (map<string, SelectionMethod*>::iterator it = selectionops.begin(); it != selectionops.end(); ++it)
	{
		delete it->second;
	}

	for (map<string, TreeSelectionMethod*>::iterator it = treeselectionops.begin(); it != treeselectionops.end(); ++it)
	{
		delete it->second;
	}

	if (eval)
		delete eval;

	if (coeval)
		delete coeval;

	if (pop)
		delete pop;

	for (vector<vector<Function*> >::iterator it=functions.begin();it!=functions.end();++it)
	{
		for (vector<Function*>::iterator it2=it->begin(); it2!=it->end(); ++it2)
		{
			delete *it2;
		}
	}

	for (map<string, BreedPopOperator*>::iterator it = breedpop.begin(); it != breedpop.end(); ++it)
	{
		delete it->second;
	}

}

int System::gettype(string type)
{
	map<string, int>::iterator it;
	if ((it=types.find(type))!=types.end())
	{
		return it->second;
	}
	int newindex=typestrs.size();
	typestrs.push_back(type);
	types[type]=newindex;
	return newindex;
}

string System::typestr(int typeno)
{
	return typestrs[typeno];
}

void System::parseArgs(int argc, char** argv)
{

}

void System::ProcBreedPop(string index)
{
	if (breedpop.find(index)==breedpop.end())
		throw runtime_error("Invalid breed phase selected");
	breedpop[index]->operate(this);
	evaluateAll();
}

void System::ProcBreedPhase(string index)
{
	if (breed.find(index)==breed.end())
		throw runtime_error("Invalid breed phase selected");
	BreedPhasePop(pop, &(breed.at(index)));
	evaluateAll();
	updateProblemSet();
}

void System::setProblemSource(ProblemSource* source)
{
	problems=source;
	// First check if we need to add problems
	if (problems)
	{
		Problem* p;
		if (problemSetSize<0)
		{
			while (problems->newProblem(p))
				curprobs.push_back(probstats(this, p));
		}else{
			while (curprobs.size()<problemSetSize)
			{
				problems->newProblem(p);
				curprobs.push_back(probstats(this, p));
			}
		}
	}
}

void System::updateProblemSet()
{

	// Normal behaviour, swap last and average fitnesses
	for (int i=0;i<curprobs.size();++i)
	{
		curprobs[i].last=1-curprobs[i].last;
		for (int j=0;j<objectives+2;++j)
			curprobs[i].avg_fitness[curprobs[i].last][j]*=1.0/popsize;
	}

	// Check condition for changing loaded problems
	if (problems && problemSetSize>0 && problemUpdateCountdown--<=0)
	{
		problemUpdateCountdown=problemUpdateInterval;


		// TODO: Add more problemset update behaviours, for now will randomly replace 20% of set
		for (int i=0;i<curprobs.size()/5; ++i)
		{
			int replace=rand()%curprobs.size();
			if (!problems->managed)
				delete curprobs[replace].prob;
			problems->newProblem(curprobs[replace].prob);
		}
	}
}

/**
 * Constructs an index array and quicksorts the chromosomes in the list vector.
 * The sorting objective is given by the obj variable.
 *
 * @param index		The index integer vector
 * @param list		The list of individuals
 * @param obj		The objective to sort on
 * @param i		The starting index
 * @param j		The ending index
 */
void sortObjective(vector<int> &index, Population* list, int obj, int i=-1, int j=0)
{
	if (i==-1)
	{
		index.assign(list->size(), 0);
		for (size_t i=0;i<list->size();++i)
			index[i]=i;
		sortObjective(index, list, obj, 0, index.size());
		return;
	}

	if (j<=i+1) return;
	if (j==i+2)
	{
		if (list->at(index[i])->fitness[obj].weightedstd>list->at(index[i+1])->fitness[obj].weightedstd)
			swap(index[i], index[i+1]);
		return;
	}

	{
		// Move pivot value into first index
		swap(index[i], index[(i+j)/2]);
		int k,l;
		double pivotval=list->at(index[i])->fitness[obj].weightedstd;
		for(k=i+1,l=j-1;k<l;)
		{
			// Find first element greater than or equal to the pivot value
			for (;k<l && list->at(index[k])->fitness[obj].weightedstd<pivotval;++k);
			
			// Find first element less than the pivot value
			for (;k<=l && list->at(index[l])->fitness[obj].weightedstd>=pivotval;--l);

			if (k<l)
				swap(index[k], index[l]);
		}

		// Swap pivot into its proper location
		swap(index[i], index[l]);

		// Recursively sort partitions
		sortObjective(index, list, obj, i, l);
		sortObjective(index, list, obj, l+1, j);
	}
}

bool doubleeq(double a, double b)
{
	return fabs(a-b)<DOUBLE_PROX;
}

void System::computeRanks()
{
	double maxval=1;
	if (rankingMethod==PARETO)
	{
		int ranks=0;

		for (int i=0;i<pop->size();++i)
		{
			pop->at(i)->fitness[1].weightedstd=0;
			pop->at(i)->fitness[1].std=0;
		}

		// Rank the weighted fitnesses
		int rankcount=0;
		int changes=1;
		while (changes>0)
		{
			changes=0;
			for (int i=0;i<pop->size();++i)
			{
				if (!doubleeq(pop->at(i)->fitness[1].std, rankcount)) continue;
				for (int j=0;j<pop->size();++j)
				{
					if (i==j || !doubleeq(pop->at(j)->fitness[1].std, rankcount)) continue;
					int k=0;
					bool better=false;
					for (;k<objectives;++k)
					{
						if (pop->at(j)->fitness[k+2].weightedstd>pop->at(i)->fitness[k+2].weightedstd)
						{
							break;
						}
						else if (pop->at(j)->fitness[k+2].weightedstd<pop->at(i)->fitness[k+2].weightedstd)
							better=true;
					}
					if (k==objectives && better)
					{
						pop->at(i)->fitness[1].std+=1.0;
						changes++;
						break;
					}
				}
			}
			
			rankcount++;
		}
		ranks=rankcount;

		for (int i=0;i<pop->size();++i)
			pop->at(i)->fitness[1].raw=pop->at(i)->fitness[1].weightedstd=pop->at(i)->fitness[1].std;

		// Lastly, step through each program and set its absolute fitness according to number of ranks
		for (int i=0;i<pop->size();++i)
			pop->at(i)->fitness[1].weighted=1.0-(pop->at(i)->fitness[1].weightedstd)/ranks;

		// Lastly, step through each program and set its absolute fitness according to number of ranks
		maxval=ranks;
		
	}
	else if (rankingMethod=RANKEDSUM)
	{
		vector<int> index;
		vector<int> poprank;

		for (int j=0;j<pop->size();++j)
			pop->at(j)->fitness[1].raw=0;

		// Rank within each objective
		double weightsum=0;
		for (int i=0;i<objectives;++i)
		{
			poprank.assign(pop->size(), 0);
			sortObjective(index, pop, 2+i);
			int rank=0;
			for (int j=1;j<pop->size();++j)
			{
				if (!doubleeq(pop->at(index[j])->fitness[2+i].weightedstd, pop->at(index[j-1])->fitness[2+i].weightedstd))
					rank=(rankPosition?j:rank+1);
				poprank[index[j]]=rank;
			}
			for (int j=0;j<pop->size();++j)
			{
				double factor=(objectiveWeight.size()>i?objectiveWeight[i]:1.0);
				if (rankNormalize)
					factor=factor*1.0/(rank==0?1:rank);
				pop->at(j)->fitness[1].raw+=poprank[j]*factor;
			}
			weightsum+=(objectiveWeight.size()>i?objectiveWeight[i]:1);
		}
		
		maxval=weightsum;
		for (int j=0;j<pop->size();++j)
		{
			pop->at(j)->fitness[1].weightedstd=pop->at(j)->fitness[1].std=pop->at(j)->fitness[1].raw;
		}
	}
	else if (rankingMethod==WEIGHTED)
	{
		// The number of ranks is equal to the population size. The
		// population is already sorted according to standard fitness,
		// so these ranks are easily computable
		int ranks=pop->size();

		for (int i=0;i<pop->size();++i)
		{
			pop->at(i)->fitness[1].raw=pop->at(i)->fitness[1].std=i;
			pop->at(i)->fitness[1].abs=1.0-(double)i/(ranks-1);
		}

		// Weighted ranks will be calculated using a simple # of items
		// less than algorithm
		
		for (int i=0;i<pop->size();++i)
		{
			pop->at(i)->fitness[1].weightedstd=0.0;

			// Count number of individuals with a better weighted fitness
			for (int j=0;j<pop->size();++j)
			{
				if (pop->at(j)->fitness[0].weightedstd < pop->at(i)->fitness[0].weightedstd)
					pop->at(i)->fitness[1].weightedstd+=1.0;
			}
			
			pop->at(i)->fitness[1].weighted=1.0-pop->at(i)->fitness[1].weightedstd/(ranks-1);
		}
		maxval=ranks-1;
	}

	// If the ranking method is not weighted, then we probably want the population sorted according
	// to the computed ranks, so these computed ranks should be copied to index 0
	if (objectives>0 && rankingMethod!=WEIGHTED)
	{
		if (rankDiversityMultiplier>0)
		{
			for (int i=1;i<pop->size();++i)
			{
				for (int j=i-1;j<i;++j)
				{
					int k;
					for (k=2;k<2+objectives;++k)
					{
						if (fabs(pop->at(i)->fitness[k].std-pop->at(j)->fitness[k].std)>0.000001)
							break;
					}
					if (k==2+objectives)
					{
						pop->at(i)->fitness[1].std+=rankDiversityMultiplier;
						pop->at(i)->fitness[1].weightedstd+=rankDiversityMultiplier;
						if (pop->at(i)->fitness[1].std>maxval)
							maxval=pop->at(i)->fitness[1].std;
					}
				}
			}
		}
		maxval=1.0/maxval;
		for (int i=0;i<pop->size();++i)
		{
			pop->at(i)->fitness[1].abs=1.0-(pop->at(i)->fitness[1].std)*maxval;
			pop->at(i)->fitness[1].weighted=pop->at(i)->fitness[1].abs;
		}

		for (int i=0;i<pop->size();++i)
		{
			pop->at(i)->fitness[0]=pop->at(i)->fitness[1];
		}
	}
}

void System::evaluateAll()
{
	pop->unsort();	// This is just to build a valid array in the sorted pop array
	if (coev_phase)
	{
		((BreedPopOperator*)(coev_phase))->operate(this);
	}else{
		for (int i=0;i<pop->size();++i)
			evaluate((Program*)(pop->at(i)));
	}

	if (rankingMethod==WEIGHTED)
		pop->sort();
	computeRanks();
	if (rankingMethod!=WEIGHTED)
		pop->sort();

	// Compute population averages
	vector<Population*> popavg;
	popavg.push_back(pop);
	while (!popavg.empty())
	{
		Population* p=popavg.back();
		popavg.pop_back();
		p->fitness.assign(objectives+2, fitness_val());
		double weight=1.0/p->size();
		for (int i=0;i<p->size();++i)
		{
			// Add to population fitness
			for (int j=0;j<objectives+2;j++)
				p->fitness[j]+=p->at(i)->fitness[j]*(weight);
		}

		// Push subpops
		for (int i=0;i<p->subpops.size();++i)
		{
			popavg.push_back(p->subpops[i]);
		}
	}
}

void System::coev_evaluate(vector<Program*> &prgms)
{
	if (coeval==NULL)
		throw runtime_error("No coevolution evaluator set");
	// Initialize fitness values
	for (int i=0;i<prgms.size();++i)
		prgms[i]->fitness.assign(objectives+2, fitness_val());

	// If there are no managed problems
	if (curprobs.size()==0)
	{
		coeval->evaluate(prgms);
		for (int i=0;i<objectives+2;++i)
		{
			for (int j=0;j<prgms.size();++j)
				prgms[j]->fitness[i].weighted=prgms[j]->fitness[i].abs;
		}
	}else{
		// Otherwise, evaluate and track stats on each loaded problem, no weighting yet

		vector<vector<fitness_val> > fit;
		double probweight=1.0/curprobs.size();	
		fit.assign(prgms.size(), vector<fitness_val>());
		for (int k=0;k<prgms.size();++k)
			fit[k].assign(objectives+2, fitness_val());
		for (int i=0;i<curprobs.size();++i)
		{
			coeval->evaluate(prgms, curprobs[i].prob);
			for (int k=0;k<prgms.size();++k)
			{
				for (int j=0;j<objectives+2;++j)
				{
					// Calculate current problem's weighted fitness
					prgms[k]->fitness[j].weighted=prgms[k]->fitness[j].abs;
					prgms[k]->fitness[j].weightedstd=prgms[k]->fitness[j].std;

					// Add to fitness value
					fit[k][j]+=prgms[k]->fitness[j];
				}
			}
		}

		// Calculate averages over all tested problem instances
		for (int k=0;k<prgms.size();++k)
		{
			for (int i=0;i<objectives+2;++i)
			{
				prgms[k]->fitness[i].weighted=fit[k][i].weighted*probweight;
				prgms[k]->fitness[i].weightedstd=fit[k][i].weightedstd*probweight;
				prgms[k]->fitness[i].abs=fit[k][i].abs*probweight;
				prgms[k]->fitness[i].std=fit[k][i].std*probweight;
				prgms[k]->fitness[i].raw=fit[k][i].raw*probweight;
			}
		}	

	}
}


void System::evaluate(Program* prgm)
{
	if (eval==NULL)
		throw runtime_error("No evaluator set");
	// Initialize fitness values
	prgm->fitness.assign(objectives+2, fitness_val());

	// If there are no managed problems
	if (curprobs.size()==0)
	{
		eval->evaluate(prgm);
		for (int i=0;i<objectives+2;++i)
		{
			prgm->fitness[i].weightedstd=prgm->fitness[i].std;
			prgm->fitness[i].weighted=prgm->fitness[i].abs;
		}
	}else{
		// Otherwise, evaluate and track stats on each loaded problem

		vector<double> ttlweight;	// Sum up the weights of each problem
		vector<fitness_val> fit;
		double curweight;
		double probweight=1.0/curprobs.size();
		ttlweight.assign(objectives+2, 0.0);
		fit.assign(objectives+2, fitness_val());
		for (int i=0;i<curprobs.size();++i)
		{
			eval->evaluate(prgm, curprobs[i].prob);
			for (int j=0;j<objectives+2;++j)
			{
				// Add current fitness for next generation's use
				curprobs[i].avg_fitness[1-curprobs[i].last][j]+=prgm->fitness[j].abs*probweight;

				// Calculate weight resulting from last generation
				curweight=1.0+weighted*(1.0-curprobs[i].avg_fitness[curprobs[i].last][j]);
				ttlweight[j]+=curweight;

				// Calculate current problem's weighted fitness
				prgm->fitness[j].weighted=prgm->fitness[j].abs*curweight;
				prgm->fitness[j].weightedstd=prgm->fitness[j].std*curweight;

				// Add to fitness value
				fit[j]+=prgm->fitness[j];
			}
		}

		// Calculate averages over all tested problem instances
		for (int i=0;i<objectives+2;++i)
		{
			if (fabs(ttlweight[i])<0.001) ttlweight[i]=0.001;
			prgm->fitness[i].weighted=fit[i].weighted/ttlweight[i];
			prgm->fitness[i].weightedstd=fit[i].weightedstd/ttlweight[i];
			prgm->fitness[i].abs=fit[i].abs*probweight;
			prgm->fitness[i].std=fit[i].std*probweight;
			prgm->fitness[i].raw=fit[i].raw*probweight;
		}

	}
}

void System::BreedPhasePop(Population* p, BreedPhase* b)
{
	AlteredPopulation* apop=dynamic_cast<AlteredPopulation*>(p);

	// If there are individuals in this population, we can do the breeding
	if (p->programs.size()>0 || p->subpops.size()==0)
	{
		b->breed(this, p, p);

		// After producing new programs, they need to be evaluated, this is done in a later phase
	}

	// Breed subpopulations
	int spnum=0;
	for (vector<Population*>::iterator it=p->subpops.begin();it!=p->subpops.end();++it)
	{
		if (apop)
		{
#ifdef DEBUG
	if (apop->functions.size()<spnum)
	{
		throw runtime_error("Altered population does not have enough function altering branches for how many subpopulations it contains.");
	}
#endif
			// Push the functions of altered architectures onto the function
			// list so that programs working with them don't need to know the
			// difference.
			for (int i=0;i<apop->functions[spnum].size();++i)
			{
				functions[apop->functions[spnum][i]->arg_type[0]].push_back(apop->functions[spnum][i]);
				adf.push_back(apop->functions[spnum][i]);
			}
		}

		BreedPhasePop(*it, b);

		if (apop)
		{
			for (int i=apop->functions[spnum].size()-1;i>=0;--i)
			{
				functions[apop->functions[spnum][i]->arg_type[0]].pop_back();
				adf.pop_back();
			}
		}
		spnum++;
	}

}

void System::parseStream(istream* input)
{
	XmlReader xml;
	xml.open(input);
	xml.read();
	if (xml.get_name()!="gp")
		throw runtime_error("Expected initial tag in XML file to be <gp>, found "+xml.get_name());

	string tagname;
	int lineno;
	BreedPopOperator* bp;
	// Read entire tag
	popsize=-1;
	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		tagname=xml.get_name();
		lineno=xml.get_line_no();
		if (xml.get_name()=="population")
		{
			popsize=atoi(xml.get_attribute("value").c_str());
		}
		else if (xml.get_name()=="maxnodes")
		{
			maxnodes=atoi(xml.get_attribute("value").c_str());
		}
		else if (xml.get_name()=="maxdepth")
		{
			maxdepth=atoi(xml.get_attribute("value").c_str());
		}

		else if (xml.get_name()=="breed")
		{
			string pname=xml.get_attribute("name");
			if (pname=="")
				throw runtime_error("Breed phase at line "+to_string(xml.get_line_no())+" needs name");
			if (breed.find(pname)!=breed.end())
				throw runtime_error("Population operation with this name already exists, duplicate at line "+to_string(xml.get_line_no()));
			breed[pname]=BreedPhase();
			breed[pname].parseXML(xml, this);		
		}
		else if (bp=getBreedPopOperator(xml.get_name()))
		{
			string pname=xml.get_attribute("name");
			if (pname=="")
				throw runtime_error("Population operation at line "+to_string(xml.get_line_no())+" needs name");
			if (breedpop.find(pname)!=breedpop.end())
				throw runtime_error("Population operation with this name already exists, duplicate at line "+to_string(xml.get_line_no()));
			bp = bp->clone();
			bp->parseXML(xml, this);
			breedpop[pname]=bp;
			bp->initialize(this);
		}
		else if (xml.get_name()=="#text")
		{
			// Text blocks are usually comments
		}
		else{
			cerr << "Ignoring unrecognized tag "<<xml.get_name()<<" on line "<<xml.get_line_no()<<endl;
			for(int sdepth=xml.get_depth();xml.read() && xml.get_depth()>sdepth;);
		}

		if (xml.get_depth()>depth+1)
		{
			cerr << "Warning, last XML tag "<<tagname<<" on line "<<lineno<<" was not read to completion"<<endl;
			while (xml.read() && xml.get_depth()>depth+1);
		}
	}
	if (popsize<=0)
		throw runtime_error("Error, no population size set in XML file.");
	pop->setSize(popsize);
}

void System::parseFile(string filename)
{
	istream* in = new ifstream(filename.c_str());
	parseStream(in);
	delete in;
}

bool System::registerBreedPopOperator(BreedPopOperator* breedop)
{
	// Check to make sure this breed operator name is not already registered
	if (breedpopops.insert(make_pair(breedop->name, breedop)).second==false)
	{
		cerr << "Warning, a population breed operator named "<<breedop->name<<" was already registered, new definition ignored" << endl;
		delete breedop;
		return false;
	}
	return true;
}

bool System::addFunction(Function* func)
{
	ADF* adf=dynamic_cast<ADF*>(func);
	if (adf!=NULL)
	{
		adf->adf_no = adfs++;
		this->adf.push_back(adf);
	}

	while (functions.size()<=func->arg_type[0])
		functions.push_back(vector<Function*>());
	functions[func->arg_type[0]].push_back(func);
}

BreedPopOperator* System::getBreedPopOperator(string name)
{
	map<string,BreedPopOperator*>::iterator it=breedpopops.find(name);
	if (it==breedpopops.end())
		return NULL;
	return it->second;
}

bool System::registerBreedOperator(BreedOperator* breedop)
{
	// Check to make sure this breed operator name is not already registered
	if (breedops.insert(make_pair(breedop->name, breedop)).second==false)
	{
		cerr << "Warning, a breed operator named "<<breedop->name<<" was already registered, new definition ignored" << endl;
		delete breedop;
		return false;
	}
	return true;
}

BreedOperator* System::getBreedOperator(string name)
{
	map<string,BreedOperator*>::iterator it=breedops.find(name);
	if (it==breedops.end())
		return NULL;
	return it->second;
}

bool System::registerSelectionMethod(SelectionMethod* sel)
{
	// Check to make sure this breed operator name is not already registered
	if (selectionops.insert(make_pair(sel->name, sel)).second==false)
	{
		cerr << "Warning, a seleciton method named "<<sel->name<<" was already registered, new definition ignored" << endl;
		delete sel;
		return false;
	}
	return true;
}

SelectionMethod* System::getSelectionMethod(string name)
{
	map<string,SelectionMethod*>::iterator it=selectionops.find(name);
	if (it==selectionops.end())
		return NULL;
	return it->second;
}

bool System::registerTreeSelectionMethod(TreeSelectionMethod* sel)
{
	// Check to make sure this breed operator name is not already registered
	if (treeselectionops.insert(make_pair(sel->name, sel)).second==false)
	{
		cerr << "Warning, a tree selection method named "<<sel->name<<" was already registered, new definition ignored" << endl;
		delete sel;
		return false;
	}
	return true;
}
TreeSelectionMethod* System::getTreeSelectionMethod(string name)
{
	map<string,TreeSelectionMethod*>::iterator it=treeselectionops.find(name);
	if (it==treeselectionops.end())
		return NULL;
	return it->second;
}


