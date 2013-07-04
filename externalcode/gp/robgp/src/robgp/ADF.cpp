#include "ADF.hpp"
#include "Utility.hpp"
#include <stdexcept>
using namespace std;
using namespace robgp;

ADF::ADF(vector<int>& argtypes, string name, int adf_no)
{
	this->name=name;
	arg_type.assign(argtypes.begin(), argtypes.end());
	this->adf_no = adf_no;

	for (int i=1;i<argtypes.size();++i)
		args.push_back(new ADFarg(i-1, arg_type[i]));
}

ADF::~ADF()
{
	for (int i=1;i<args.size();++i)
		delete args[i];
}

void ADF::rename()
{
	this->name="ADF"+to_string(adf_no);
}

Function* ADF::clone()
{
	ADF* c = new ADF(arg_type, name, adf_no);
	return c;
}

Datatype ADF::eval(TreeNodeIter curnode, Frame* frame)
{
	/* If the system hit the recursive ADF depth limit, then it will
	 * not enter another ADF. Instead it will return a zero value
	 */
	if (frame->maxdepth>0 && frame->depth==frame->maxdepth)
	{
		return Datatype();
	}

	vector<Datatype> argvals=evalAll(curnode, frame);

#ifdef DEBUG
	if (argvals.size()!=arg_type.size()-1)
	{
		throw runtime_error("Incorrect number of arguments supplied to function");
	}
#endif

	frame->push(argvals);
	TreeNodeIter adfnode=frame->adfs[adf_no];

	Datatype ret=(*adfnode).f->eval(adfnode, frame);
	frame->pop();
	return ret;
}

void ADF::write(ostream& out, TreeNodeIter curnode)
{
	if (name=="")
		out << "ADF" << adf_no;
	else
		out << name;
	out << "(";
	TreeNodeIter it=curnode.begin();
	if (it!=curnode.end())
	{
		(*it).f->write(out, it);
		++it;
	}
	for (;it!=curnode.end();++it)
	{
		out << ", ";
		(*it).f->write(out, it);
	}
	out << ")";
}

TreeNode ADF::createInstance()
{
	TreeNode t=Function::createInstance();
	t.extra.v_int=adf_no;
	return t;
}



