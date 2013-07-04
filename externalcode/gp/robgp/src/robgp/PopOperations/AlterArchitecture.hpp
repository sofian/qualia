#pragma once
#include "../BreedPopOperator.hpp"
#include "../SelectionMethod.hpp"
#include "../TreeGrower.hpp"

namespace robgp
{

namespace PopOperations
{

/**
 * This class implements Koza's architecture altering operations, in a
 * slightly modified manner to eliminate problems of incompatible crossover.
 * The population is split up into groups and each group has a common
 * architecture. When breeding the architectures, one group overwrites another
 * with a modified architecture. All crossover takes place within the
 * architecture group so that there are no incompatible program branches
 * copied.
 */

class AlterArchitecture : public BreedPopOperator
{
public:
	AlterArchitecture();
	~AlterArchitecture();
	
	virtual void initialize(System* system);

	virtual BreedPopOperator* clone();
	virtual void parseXMLNode(XmlReader& xml, System* system);
	virtual void parseXMLAttribute(XmlReader& xml, System* system);
	virtual void operate(System* system, Population* pop);
	
protected:
	TreeGrower* getGrower();

private:
 	void duplicateFunc(Population* p, vector<Function*> *funcs, int func_no);
	TreeGrower* grower;

	int maxadfs;
	int maxargs;
};

}};

