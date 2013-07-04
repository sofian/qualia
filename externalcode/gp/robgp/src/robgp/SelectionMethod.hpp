#pragma once
#include "Selectable.hpp"
#include "TreeSelectionMethod.hpp"
#include "XmlReader.hpp"
#include <vector>
#include <string>

namespace robgp
{
	using namespace std;
	class System;

	class SelectionMethod
	{
	public:
		SelectionMethod();
		virtual ~SelectionMethod();

		string name;
		int index;

		virtual Selectable* select(SelectableList* choices, int position, Selectable* last=NULL);

		virtual SelectionMethod* clone()=0;
		virtual void parseXML(XmlReader& xml, System* system);
		virtual void parseXMLNode(XmlReader& xml, System* system);
		virtual void parseXMLAttribute(XmlReader& xml, System* system);

		int selectTree(int branch, Program* p, Program* prev, int prevtree, int type=-1);
	protected:
		bool reverse;

		inline double chance(double val)
		{
			if (reverse)
				return 1.0-val;
			else
				return val;
		}
	private:
		TreeSelectionMethod* treeSelect;
		float internalPercent;
	};

};

