#include "RouletteWheel.hpp"
#include <cstdlib>
using namespace std;
using namespace robgp;
using namespace robgp::Selection;

RouletteWheel::RouletteWheel() : SelectionMethod()
{
	name="roulette";
}

RouletteWheel::~RouletteWheel()
{

}

SelectionMethod* RouletteWheel::clone()
{
	RouletteWheel* c=new RouletteWheel();
	*c=*this;
	return c;
}

Selectable* RouletteWheel::select(SelectableList* choices, int position, Selectable* prev)
{
	// First determine sum of choices eligibility
	double total=0.0;
	for (int i=0;i<choices->size();++i)
		total+=chance(choices->at(i)->fitness[index].weighted);

	double choice=(double)rand()/RAND_MAX*total;

	for (int i=0;i<choices->size();++i)
	{
		choice-=chance(choices->at(i)->fitness[index].weighted);
		if (choice<0)
			return choices->at(i);
	}

	return choices->at(choices->size()-1);
}

void RouletteWheel::parseXMLNode(XmlReader& xml, System* system)
{
	SelectionMethod::parseXMLNode(xml, system);
}


