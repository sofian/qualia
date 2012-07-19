#include "BreedPopOperator.hpp"
#include "System.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace robgp;

BreedPopOperator::BreedPopOperator()
{
	affectedDepth=-1;
	groups = 4;
	name = "Uninitialized BreedPopOperator";
}

BreedPopOperator::~BreedPopOperator()
{
	for (vector<SelectionMethod*>::iterator it = selection.begin(); it!=selection.end(); ++it)
		delete *it;

}

void BreedPopOperator::parseXML(XmlReader& xml, System* system)
{
	parseXMLNode(xml, system);
	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		if (xml.get_name()!="#text")
			parseXMLAttribute(xml, system);
	}
}

void BreedPopOperator::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("groups")!="")
		groups=atoi(xml.get_attribute("groups").c_str());
}

void BreedPopOperator::parseXMLAttribute(XmlReader& xml, System* system)
{
	if (xml.get_name()=="select")
	{
		for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
		{
			if (xml.get_name()!="#text")
			{
				SelectionMethod* sel=system->getSelectionMethod(xml.get_name());
				if (sel==NULL)
					throw runtime_error("No selection method named "+(xml.get_name())+" is registered with the system");
				sel=sel->clone();
				sel->parseXML(xml, system);
				selection.push_back(sel);
			}
		}
	}else
		cerr << "Warning, unrecognized node "<<xml.get_name()<<" at line "<<xml.get_line_no()<<endl;
}

void BreedPopOperator::initialize(System* system)
{
	// Split population into correct number of groups
	affectedPop = system->pop = new Population(system->pop, groups);
}

void BreedPopOperator::calcAffectedDepth(System* system)
{
	if (affectedDepth<0)
	{
		affectedDepth=0;
		Population* p = system->pop;
		while (p!=affectedPop)
		{
			p=p->subpops[0];
			affectedDepth++;
		}
	}
}

void BreedPopOperator::operateDepth(System* system, Population* pop, int depth)
{
	if (depth==0)
		operate(system, pop);
	else
	{
		for (int i=0;i<pop->subpops.size();++i)
			operateDepth(system, pop->subpops[i], depth-1);
	}
}

void BreedPopOperator::operate(System* system)
{
	// First determine affected depth
	calcAffectedDepth(system);
	
	// Then find all populations at that depth, and send them one by one to be processed
	operateDepth(system, system->pop, affectedDepth);
}

