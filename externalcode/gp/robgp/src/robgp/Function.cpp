#include "Function.hpp"
#include "TreeNode.hpp"
#include <cstdlib>

using namespace robgp;
using namespace std;

Function::Function() : 
	minargs(0),
	maxargs(-1)
{
}

Function::~Function()
{

}

TreeNode Function::createInstance()
{
	TreeNode t(this);
	if (maxargs==-1)
		t.arity=arg_type.size()-1;
	else
		t.arity=rand()%(maxargs-minargs+1)+minargs;
	return t;
}

void Function::write(ostream& out, TreeNodeIter curnode)
{
	out << "(";
	out << name;
	for (TreeNodeIter it=curnode.begin();it!=curnode.end();++it)
	{
		out << " ";
		(*it).f->write(out, it);
	}
	out << ")";
}

vector<Datatype> Function::evalAll(TreeNodeIter curnode, Frame* frame)
{
	vector<Datatype> retargs;
	for (TreeNodeIter it=curnode.begin();it!=curnode.end();++it)
	{
		retargs.push_back(it->f->eval(it, frame));
	}
	return retargs;
}

int Function::max_args()
{
	if (maxargs>=0)
		return maxargs;
	return arg_type.size()-1;
}

int Function::min_args()
{
	if (maxargs>=0)
		return minargs;
	return arg_type.size()-1;
}

