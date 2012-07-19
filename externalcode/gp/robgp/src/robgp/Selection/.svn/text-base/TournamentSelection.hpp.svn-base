#pragma once

#include "../SelectionMethod.hpp"

namespace robgp
{
namespace Selection
{
	class TournamentSelection : public SelectionMethod
	{
	public:
		TournamentSelection(int tournamentSize=3);
		~TournamentSelection();

		virtual Selectable* select(SelectableList* choices, int position, Selectable* prev=NULL);

		virtual SelectionMethod* clone();
		virtual void parseXMLNode(XmlReader& xml, System* system);
		
		int k;
	};
}};

