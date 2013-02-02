#pragma once

#include "../SelectionMethod.hpp"

namespace robgp
{
namespace Selection
{
	class RouletteWheel : public SelectionMethod
	{
	public:
		RouletteWheel();
		~RouletteWheel();

		virtual Selectable* select(SelectableList* choices, int position, Selectable* prev=NULL);

		virtual SelectionMethod* clone();
		virtual void parseXMLNode(XmlReader& xml, System* system);
		
	};
}};

