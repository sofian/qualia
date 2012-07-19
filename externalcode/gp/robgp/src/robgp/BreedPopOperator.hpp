#pragma once
#include "XmlReader.hpp"
#include "Population.hpp"
#include "SelectionMethod.hpp"

namespace robgp
{

class System;

class BreedPopOperator
{
public:
	BreedPopOperator();
	virtual ~BreedPopOperator();

	virtual BreedPopOperator* clone()=0;

	virtual void parseXML(XmlReader& xml, System* system);
	virtual void parseXMLNode(XmlReader& xml, System* system);
	virtual void parseXMLAttribute(XmlReader& xml, System* system);

	virtual void initialize(System* system);
	virtual void operate(System* system, Population* pop)=0;
	void operate(System* system);

	/**
	 * up to srcTrees selection methods for choosing the source programs.
	 */
	vector<SelectionMethod*> selection;

	string name;

protected:
	Population* affectedPop;
	int groups;
private:
	void operateDepth(System* system, Population* pop, int depth);
	void calcAffectedDepth(System* system);
	int affectedDepth;
};

}

