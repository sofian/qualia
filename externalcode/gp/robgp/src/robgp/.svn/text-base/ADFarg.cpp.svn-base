#include "ADFarg.hpp"
#include "Utility.hpp"
#include <stdexcept>

using namespace std;
using namespace robgp;

ADFarg::ADFarg(int argno, int type)
{
	argNo = argno;
	arg_type.push_back(type);
	name="ARG"+to_string(argno);
}

ADFarg::~ADFarg()
{
    
}

Datatype ADFarg::eval(TreeNodeIter curnode, Frame* frame)
{
#ifdef DEBUG
	if (argNo>=frame->args->size())
		throw runtime_error("Attempt to fetch argument number past size of argument list");
#endif
	return frame->args->at(argNo);
}
