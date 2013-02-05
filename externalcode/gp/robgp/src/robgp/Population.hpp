#pragma once
#include <vector>
#include "Program.hpp"

namespace robgp
{

using namespace std;

/**
 * This class represents a population of GP individuals. While this could be
 * as simple as a list of programs, it also includes their specific ADF list.
 * This is the improvised solution to the many problems with the architecture
 * altering operations described in Koza-3. This solution has a fixed
 * function set for each sub-population, which can be evolved between
 * sub-populations.
 */

class Population: public Selectable, public SelectableList
{
public:
	/**
	 * Construct a population with the given population size
	 *
	 * @param psize		The size of the constructed population
	 */
	Population(int psize);

	/**
	 * Construct a population by splitting the supplied subpopulation into groups
	 *
	 * @param subpop	The population to split into groups
	 * @param groups	The number of groups to split the population into
	 */
	Population(Population* subpop, int groups);
	virtual ~Population();

	/**
	 * Copy this population to the supplied destionation population including
	 * all programs.
	 *
	 * @param dst	The destination population to overwrite
	 */
	virtual void copy(Population* dst);

	/**
	 * Clone the current population object. This is not a deep copy, it is
	 * only for creating another class of the same subtype as this one.
	 */
	virtual Population* clone();

	/**
	 * Set the population size of the population.
	 *
	 * @param psize	The size of the population to use.
	 */
	virtual void setSize(int psize);

	/**
	 * A list of the programs in this population. Normally this will be
	 * empty for all but the final population.
	 */
	vector<Program> programs;

	/**
	 * A list of subpopulations. Normally this will be empty for the
	 * final population.
	 */
	vector<Population*> subpops;

	/// An ordered list of programs in order from most fit to least fit
	vector<Program*> orderedprgms;

	/**
	 * Select an individual from the population. This will automatically
	 * select into subpopulations. Additionally this uses a fitness
	 * ordered selection where individual 0 is best.
	 *
	 * @param index	The individual to select.
	 */
	virtual Selectable* at(int index);

	/**
	 * Get the size of this population
	 *
	 * @return The size of this population
	 */
	virtual int size();

	/**
	 * Sort the population in order from most fit to least fit. This uses
	 * the absolute fitnesses, not the weighted fitnesses. The reason is so
	 * that you can have weighted fitnesses while still being able to observe
	 * the best individual absolutely.
	 */
	virtual void sort();

	/**
	 * Loads the population, including subpopulations into a local vector
	 * so that all programs can be directly referenced.
	 */
	virtual void unsort();
	
	/// The target population size
	int psize;
};

};

