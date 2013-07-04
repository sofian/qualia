#pragma once
#include "XmlReader.hpp"
#include "Population.hpp"
#include "Program.hpp"
#include "SelectionMethod.hpp"

namespace robgp
{

/**
 * This generic class will define the interface for the host of functions that
 * in some way or another produce GP programs. This includes everything from
 * tree growers to crossover to mutations.
 */

class System;

class BreedOperator
{
public:

	BreedOperator();
	virtual ~BreedOperator();

	/**
	 * The name of this breed phase. This is used for identifying it in the
	 * input configuration and so should be unique.
	 */
	string name;

	/**
	 * The number of source programs that this breed phase requires to work
	 */
	int srcTrees;

	/**
	 * The number of programs that this breed phase produces
	 */
	int dstTrees;

	float rate;

	/**
	 * up to srcTrees selection methods for choosing the source programs.
	 */
	vector<SelectionMethod*> selection;

	/**
	 * Abstract classes cannot inherently clone themselves. A template clone
	 * method could be used, but in case there are special considerations
	 * each breed operator must define a clone method.
	 */
	virtual BreedOperator* clone()=0;

	virtual int breed(System* system, Population* src, vector<Program>* newpop, int index)=0; 

	virtual void parseXML(XmlReader& xml, System* system);
	virtual void parseXMLNode(XmlReader& xml, System* system);
	virtual void parseXMLAttribute(XmlReader& xml, System* system);

protected:
	// Helper methods for breed operators

	vector<Program*> getSelections(int numselections, Population* p, int index);
	vector<int> getSelectionTrees(int branch, vector<Program*>* programs);
	virtual SelectionMethod* getSelectionMethod(int index);

};

};

