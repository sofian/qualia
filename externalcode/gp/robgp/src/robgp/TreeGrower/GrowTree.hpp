#pragma once
#include "../BreedOperator.hpp"
#include "../TreeGrower.hpp"

namespace robgp
{

namespace BreedOperators
{


class GrowTree:public TreeGrower
{
public:
	GrowTree(int minDepth=4, int maxDepth=6);
	~GrowTree();
	
	virtual int breed(System* system, Population* src, vector<Program>* newpop, int index);
	void growTree(System* system, TreeNodeIter pos, int type);
	virtual BreedOperator* clone();
	
	virtual void parseXMLNode(XmlReader& xml, System* system);


	int mindepth;
	int maxdepth;

};

}};

