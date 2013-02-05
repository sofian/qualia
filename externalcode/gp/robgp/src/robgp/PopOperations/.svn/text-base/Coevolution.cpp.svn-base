#include "Coevolution.hpp"
#include "../AlteredPopulation.hpp"
#include "../System.hpp"
#include <cstdlib>
#include <stdexcept>
#include "../Utility.hpp"
#include <cmath>
using namespace robgp;
using namespace robgp::PopOperations;

Coevolution::Coevolution() : BreedPopOperator()
{
	name = "coevolution";
}

Coevolution::~Coevolution()
{

}

void Coevolution::initialize(System* system)
{
	system->coev_phase = this;
	BreedPopOperator::initialize(system);
}

void Coevolution::operate(System* system, Population* pop)
{
	if (system->coeval == NULL)
		throw runtime_error("Error, no coevolution evaluator provided to the system");
	pop->unsort();

	// Pair up one program from each group to be evaluated together
	vector<Program*> prgms;
	prgms.assign(pop->subpops.size(), NULL);
	for (int i=0;i<pop->subpops[0]->size();++i)
	{
		for (int j=0;j<pop->subpops.size();++j)
			prgms[j]=(Program*)(pop->subpops[j]->at(i));
		system->coev_evaluate(prgms);
	}
}

BreedPopOperator* Coevolution::clone()
{
	return new Coevolution();
}

