#include "TreeSelectionMethod.hpp"
#include <stdexcept>
using namespace std;
using namespace robgp;


TreeSelectionMethod::TreeSelectionMethod()
{

}

TreeSelectionMethod::~TreeSelectionMethod()
{

}

void TreeSelectionMethod::parseXML(XmlReader& xml, System* system)
{
	parseXMLNode(xml, system);
	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		if (xml.get_name()!="#text")
			parseXMLAttribute(xml, system);
	}
}

void TreeSelectionMethod::parseXMLNode(XmlReader& xml, System* system)
{
}

void TreeSelectionMethod::parseXMLAttribute(XmlReader& xml, System* system)
{

	cerr << "Warning, unrecognized node "<<xml.get_name()<<" at line "<<xml.get_line_no()<<endl;
}

int TreeSelectionMethod::selectTree(int branch, Program* p, Program* prev, int prevsel, int type)
{
	throw runtime_error("No tree selection method programmed for this class");
}

