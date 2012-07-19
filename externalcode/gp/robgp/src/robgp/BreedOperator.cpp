#include "BreedOperator.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace robgp;

BreedOperator::BreedOperator()
{
	rate=1;
	srcTrees=dstTrees=0;
}

BreedOperator::~BreedOperator()
{
	for (vector<SelectionMethod*>::iterator it = selection.begin(); it != selection.end(); ++it)
		delete *it;
}

void BreedOperator::parseXML(XmlReader& xml, System* system)
{
	parseXMLNode(xml, system);
	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		if (xml.get_name()!="#text")
			parseXMLAttribute(xml, system);
	}
}

void BreedOperator::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("rate")!="")
		rate = atof(xml.get_attribute("rate").c_str());
}

void BreedOperator::parseXMLAttribute(XmlReader& xml, System* system)
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

SelectionMethod* BreedOperator::getSelectionMethod(int index)
{
	if (selection.size()==0)
		throw runtime_error("Selection method requested, but none have been loaded");
	return selection[index%selection.size()];
}

vector<Program*> BreedOperator::getSelections(int numselections, Population* p, int index)
{
	vector<Program*> programs;
	for (int i=0;i<numselections;++i)
	{
		programs.push_back((Program*)getSelectionMethod(i)->select(p, index-i, (i==0?NULL:(Selectable*)programs[i-1])));
	}
	return programs;
}

vector<int> BreedOperator::getSelectionTrees(int branch, vector<Program*>* programs)
{
	vector<int> subtrees;
	int seltree=0;
	int type=-1;

	for (int i=0;i<programs->size();++i)
	{
		seltree = getSelectionMethod(i)->selectTree(branch, programs->at(i), (i==0?NULL:programs->at(i-1)), seltree, type);
		subtrees.push_back(seltree);
		#ifdef DEBUG
		if (i>0 && programs->at(i)->branch[branch][seltree].f->arg_type[0]!=type)
		{
			throw runtime_error("Type of first selected branch is "+to_string(type)+" but type of next selected branch is "+to_string(programs->at(i)->branch[branch][seltree].f->arg_type[0]));
		}
		#endif
		type = programs->at(i)->branch[branch][seltree].f->arg_type[0];	// Get return type and enforce other trees to be of this type
	}

	return subtrees;
}

