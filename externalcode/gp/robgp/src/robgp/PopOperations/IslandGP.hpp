#pragma once
#include "../BreedPopOperator.hpp"
#include "../SelectionMethod.hpp"
#include "../TreeGrower.hpp"

namespace robgp
{

namespace PopOperations
{

/**
 * This is an implementation of Island GP. The population is split into groups
 * and whenever this breed phase is used, individuals from groups are allowed
 * to travel to other groups.
 */

class IslandGP : public BreedPopOperator
{
public:
	IslandGP();
	~IslandGP();
	
	virtual BreedPopOperator* clone();
	virtual void parseXMLNode(XmlReader& xml, System* system);
	virtual void parseXMLAttribute(XmlReader& xml, System* system);
	virtual void operate(System* system, Population* pop);
	
protected:
	/// The proportion of one populaton to transfer to others
	int proportion;
private:
 	void duplicateFunc(Population* p, vector<Function*> *funcs, int func_no);


};

}};

