#include "TournamentSelection.hpp"
#include <cstdlib>
using namespace std;
using namespace robgp;
using namespace robgp::Selection;

TournamentSelection::TournamentSelection(int tournamentSize) : SelectionMethod()
{
	name="tournament";
	k = tournamentSize;
}

TournamentSelection::~TournamentSelection()
{

}

SelectionMethod* TournamentSelection::clone()
{
	TournamentSelection* c=new TournamentSelection();
	*c=*this;
	return c;
}

Selectable* TournamentSelection::select(SelectableList* choices, int position, Selectable* prev)
{
	Selectable* best=choices->at(rand()%choices->size());

	for (int i=1;i<k;++i)
	{
		Selectable* test = choices->at(rand()%choices->size());
		if (chance(test->fitness[index].weightedstd)<chance(best->fitness[index].weightedstd))
			best = test;
	}
	return best;
}

void TournamentSelection::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("k")!="")
		k=atoi(xml.get_attribute("k").c_str());
	SelectionMethod::parseXMLNode(xml, system);
}


