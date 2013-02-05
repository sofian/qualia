#pragma once
#include "Population.hpp"
#include "Function.hpp"
#include <vector>

namespace robgp{
using namespace std;

class System;

class AlteredPopulation:public Population
{
public:
	AlteredPopulation(int size);
	AlteredPopulation(Population* subpop, int groups);
	~AlteredPopulation();

	virtual void copy(Population* dst);
	virtual Population* clone();
	virtual void setSize(int psize);

	virtual void prepareADFs(System* system, int subpop, int adfno);
	virtual void clearADFs(System* system, int subpop);

	/**
	 * The extra function definitions per subpopulation
	 */
	vector<vector<Function*> > functions;

protected:
	int preparedbr;

};

};

