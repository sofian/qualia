#pragma once
#include <vector>

namespace robgp
{
	using namespace std;

	class fitness_val
	{
	public:
		double raw;	/// Raw fitness
		double std;	/// Standardized fitness: [0, infinity) where 0 is best
		double abs;	/// Absolute fitness: [0, 1] where 1 is best
		double weighted;	/// Weighted absolute fitness: [0, 1] where 1 is best
		double weightedstd;	/// Weighted standard fitness: [0, infinity) where 0 is best

		/**
		 * Add all fitness values to the current fitness values
		 *
		 * @param other	The other fitness value to add to this one
		 */
		void operator+= (const fitness_val& other)
		{
			raw+=other.raw;
			std+=other.std;
			abs+=other.abs;
			weighted+=other.weighted;
			weightedstd+=other.weightedstd;
		}

		/**
		 * Multiply the fitness values by a constant scale. The combination
		 * of the addition and this will perform the average fitness.
		 *
		 * @param k	The amount to scale the fitness values by
		 */
		fitness_val operator* (double k)
		{
			fitness_val result;
			result.raw=raw*k;
			result.std=std*k;
			result.abs=abs*k;
			result.weighted=weighted*k;
			result.weightedstd=weightedstd*k;
			return result;
		}

		fitness_val()
		{
			raw=std=abs=weighted=weightedstd=0.0;
		}
	};

	/**
	 * An interface for a selectable item. Any selection method will select
	 * from a list of selectables. This is used both for selection of a
	 * population and for selection of individuals.
	 */
	class Selectable
	{
	public:
		vector<fitness_val> fitness;
	};

	/**
	 * An interface describing a list of selectable items.
	 */
	class SelectableList
	{
	public:
		virtual Selectable* at(int index)=0;
		virtual int size()=0;
	};

	class SelectableVector : public SelectableList
	{
	public:
		vector<Selectable*> list;
		virtual Selectable* at(int index)
		{
			return list[index];
		}

		virtual int size()
		{
			return list.size();
		}

	};
};

