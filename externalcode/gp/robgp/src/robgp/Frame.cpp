#include "Frame.hpp"
#include "Program.hpp"
#ifdef DEBUG
#include <iostream>
#endif
using namespace robgp;
using namespace std;

Frame::Frame(Program* prgm, int maxdepth)
{
	f_args.push(vector<Datatype>());
	args=&f_args.top();
	depth=0;
	this->maxdepth=maxdepth;
	for (int i=1;i<prgm->branch.size();++i)
	{
		adfs.push_back(TreeNodeIter(&prgm->branch[i], 0));
	}
}

Frame::~Frame()
{

}
	
void Frame::push(vector<Datatype>& newargs)
{
	++depth;
	f_args.push(newargs);
	args=&f_args.top();
}

void Frame::pop()
{
	--depth;
	f_args.pop();
	args=&f_args.top();
}

