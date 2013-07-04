#pragma once

#include "BreedOperator.hpp"

namespace robgp
{
namespace BreedOperators
{
	/**
	 * Elitism
	 */

	class Reproduction : public BreedOperator
	{
	public:
		Reproduction();
		virtual ~Reproduction();
	
		virtual BreedOperator* clone();
		virtual int breed(System* system, Population* src, vector<Program>* newpop, int index);

	};

}};

