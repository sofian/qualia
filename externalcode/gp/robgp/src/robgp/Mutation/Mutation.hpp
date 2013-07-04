#pragma once

#include "../BreedOperator.hpp"
#include "../TreeGrower.hpp"

namespace robgp
{
namespace BreedOperators
{

	/**
	 * This is the implementation of Vanilla GP mutation. It selects a
	 * subtree and regrows it using some grow operator
	 */

	class Mutation : public BreedOperator
	{
	public:
		Mutation();
		virtual ~Mutation();

		virtual BreedOperator* clone();
		virtual int breed(System* system, Population* src, vector<Program>* newpop, int index);
		virtual void parseXMLAttribute(XmlReader& xml, System* system);

		TreeGrower* grower;
	};

}};

