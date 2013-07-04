#pragma once

#include "../BreedOperator.hpp"

namespace robgp
{
namespace BreedOperators
{

	/**
	 * The vanilla GP crossover which uses tree-based crossover. You can
	 * specify a subtree selection method to turn this into.
	 */

	class Crossover : public BreedOperator
	{
	public:
		Crossover();
		virtual ~Crossover();
	
		virtual BreedOperator* clone();
		virtual int breed(System* system, Population* src, vector<Program>* newpop, int index);

	};

}};


