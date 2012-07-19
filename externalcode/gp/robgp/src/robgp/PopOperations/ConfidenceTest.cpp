#include "ConfidenceTest.hpp"
#include "../AlteredPopulation.hpp"
#include "../System.hpp"
#include <cstdlib>
#include <stdexcept>
#include "../Utility.hpp"
#include <cmath>
using namespace robgp;
using namespace robgp::PopOperations;

// TODO: Modify confidence test to multiply population size by number of groups
// rather than splitting it.

ConfidenceTest::ConfidenceTest() : BreedPopOperator()
{
	name = "confidencetest";
}

ConfidenceTest::~ConfidenceTest()
{

}

void ConfidenceTest::initialize(System* system)
{
	// Increase the population so that the population for group is the old population size
	system->popsize*=groups;
	BreedPopOperator::initialize(system);
}

void ConfidenceTest::operate(System* system, Population* pop)
{
	mean.assign(system->objectives+2, 0.0);
	stddev.assign(system->objectives+2, 0.0);

	for (int j=0;j<system->objectives+2;++j)
	{
		for (int i=0;i<pop->size();++i)
		{
			mean[j]+=pop->at(i)->fitness[j].raw;
		}
		mean[j]=mean[j]/pop->size();
	}

	double diff;
	for (int j=0;j<system->objectives+2;++j)
	{
		for (int i=0;i<pop->size();++i)
		{
			diff=pop->at(i)->fitness[j].raw-mean[j];
			stddev[j]+=diff*diff;
		}
		stddev[j]=sqrt(stddev[j]/pop->size());
	}

	
}

BreedPopOperator* ConfidenceTest::clone()
{
	return new ConfidenceTest();
}

