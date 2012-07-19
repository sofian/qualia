#include "IslandGP.hpp"
#include "../AlteredPopulation.hpp"
#include "../System.hpp"
#include <cstdlib>
#include <stdexcept>
#include <typeinfo>
#include "../Utility.hpp"
using namespace robgp;
using namespace robgp::PopOperations;

IslandGP::IslandGP() : BreedPopOperator()
{
	name = "islandgp";
	proportion=10;
}

IslandGP::~IslandGP()
{

}

void IslandGP::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("portion")=="")
		proportion=atoi(xml.get_attribute("portion").c_str());
}

void IslandGP::parseXMLAttribute(XmlReader& xml, System* system)
{
	BreedPopOperator::parseXMLAttribute(xml, system);
}

void IslandGP::operate(System* system, Population* pop)
{
	// Select the proportion% best individuals from each population and
	// distribute them to the other populations.

	if (selection.size()==0)
		throw runtime_error("No selection methods chosen for individual selection and replacement.");

	// Determine number of individuals to transfer
	int individuals=pop->size()*proportion/100;
	int dstpop;

	for (int subpop=0;subpop<pop->subpops.size();subpop++)
	{
			for (int i=0;i<individuals;++i)
			{
				dstpop=(subpop+i)%pop->subpops.size();
				Program* src_p = (Program*)(selection[0]->select(pop->subpops[subpop], i));
				Program* dst_p = (Program*)(selection[1%selection.size()]->select(pop->subpops[dstpop], subpop));
				
				src_p->copy(*dst_p);
			}
	}
}

BreedPopOperator* IslandGP::clone()
{
	return new IslandGP();
}

