#include "SelectionMethod.hpp"
#include "System.hpp"
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace robgp;

SelectionMethod::SelectionMethod()
{
	index=0;
	treeSelect=NULL;
	internalPercent=0.6;
	reverse=false;
}

int SelectionMethod::selectTree(int branch, Program* p, Program* prev, int prevtree, int type)
{
	// Provide a default just in case the user hasn't set everything up
	if (treeSelect==NULL)
	{
	//		return rand()%(p->branch[branch].size());
		bool internal=false;
		// Give random chance to choose internal node
		if ((float)rand()/RAND_MAX < internalPercent)
		{
			internal=true;
		}

		int nodecount=0;
		int i;
		int tries=0;
		while(true)
		{
			tries++;
			nodecount=0;
			for (i=0;i<p->branch[branch].size();++i)
			{
				if ((type<0 || p->branch[branch][i].f->arg_type[0]==type) && ((!internal && p->branch[branch][i].arity==0) ||
					(internal && p->branch[branch][i].arity>0)))
					nodecount++;
			}
			if (nodecount>0)
				break;
			if (tries==2)
				throw BreedError("Found a tree without target type!");
			internal=!internal;
		}

		int selnode=rand()%nodecount;
		for (i=0;i<p->branch[branch].size();++i)
		{
			if ((type<0 || p->branch[branch][i].f->arg_type[0]==type) && ((!internal && p->branch[branch][i].arity==0) ||
				(internal && p->branch[branch][i].arity>0)))
				if (selnode--==0)
					break;
		}
		if (i>=p->branch[branch].size())
			throw runtime_error("Uhoh, couldn't find the node we were looking for");
		return i;
	}else{
		return treeSelect->selectTree(branch, p, prev, prevtree, type);
	}
}

SelectionMethod::~SelectionMethod()
{
	if (treeSelect!=NULL)
		delete treeSelect;
}

Selectable* SelectionMethod::select(SelectableList* choices, int position, Selectable* prev)
{
	throw runtime_error("No selection method programmed for this class");
}

void SelectionMethod::parseXML(XmlReader& xml, System* system)
{
	parseXMLNode(xml, system);
	for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
	{
		if (xml.get_name()!="#text")
			parseXMLAttribute(xml, system);
	}
}

void SelectionMethod::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("objective")!="")
	{
		if (xml.get_attribute("objective")=="weighted")
			index=0;
		else if (xml.get_attribute("objective")=="ranked")
			index=1;
		else
			index=atoi(xml.get_attribute("objective").c_str())+1;
	}else if (xml.get_attribute("internal")!="")
		internalPercent=atof(xml.get_attribute("internal").c_str());
	else if (xml.get_attribute("reverse")!="")
		reverse=true;
}

void SelectionMethod::parseXMLAttribute(XmlReader& xml, System* system)
{
	if (xml.get_name()=="TreeSelect")
	{
		TreeSelectionMethod* ts=system->getTreeSelectionMethod(xml.get_attribute("method"));
		if (ts==NULL)
			throw runtime_error("Could not find registered tree selection method: "+xml.get_attribute("method"));
		ts=ts->clone();
		ts->parseXML(xml, system);
		treeSelect=ts;
	}else
		cerr << "Warning, unrecognized node "<<xml.get_name()<<" at line "<<xml.get_line_no()<<endl;
}

