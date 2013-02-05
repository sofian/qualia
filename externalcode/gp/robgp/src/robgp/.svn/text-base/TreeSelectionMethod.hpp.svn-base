#pragma once

#include "Program.hpp"
#include "XmlReader.hpp"
#include <vector>
#include <string>

namespace robgp
{
	using namespace std;
	class System;

	class TreeSelectionMethod
	{
	public:
		TreeSelectionMethod();
		virtual ~TreeSelectionMethod();

		string name;
		virtual TreeSelectionMethod* clone()=0;
		virtual void parseXML(XmlReader& xml, System* system);
		virtual void parseXMLNode(XmlReader& xml, System* system);
		virtual void parseXMLAttribute(XmlReader& xml, System* system);

		virtual int selectTree(int branch, Program* p, Program* prev, int prevsel, int type=-1);
	};

};

