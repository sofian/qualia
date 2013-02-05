#include "Best.hpp"
#include <cstdlib>
using namespace std;
using namespace robgp;
using namespace robgp::Selection;

Best::Best() : SelectionMethod()
{
	name="best";
}

Best::~Best()
{

}

SelectionMethod* Best::clone()
{
	Best* c=new Best();
	*c=*this;
	return c;
}

Selectable* Best::select(SelectableList* choices, int position, Selectable* prev)
{
	return choices->at(position);
}



