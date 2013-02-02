#pragma once
#include <vector>
#include "XmlReader.hpp"
#include "BreedOperator.hpp"
#include "Population.hpp"

namespace robgp
{

using namespace std;

class System;

/**
 * This generic class will define a list of breed operators and operate them
 * at some rate.
 */

class BreedPhase
{
public:

	BreedPhase();
	~BreedPhase();

	/**
	 * Parse the XML stream to construct a breed phase.
	 *
	 * @param xml		The xml reader stream
	 * @param system	The GP system object
	 */
	void parseXML(XmlReader& xml, System* system);

	/**
	 * Breed a population using this breed phase. The entire population
	 * in the destination will be replaced by newly constructed individuals.
	 *
	 * @param system	The GP system object
	 * @param src		The source population to select from
	 * @param dest		The destination population to overwrite. This
	 *			can be the same as the source population. The
	 *			system only replaces it on completion of
	 *			breeding.
	 */
	void breed(System* system, Population* src, Population* dest);

	/**
	 * The loaded breed operators. Each one has a local rate that gives
	 * how frequently it runs with respect to the others.
	 */
	vector<BreedOperator*> breedops;	

	/**
	 * Call this function to inform the breed phase that you have updated the
	 * rates for one or more breed operators, so that it can update the total
	 * rate.
	 */
	void updateRates();

private:
	/// The combined total of all the breed operator rates
	float totalrate;
};

};


