#include "Problem.hpp"
#include <cstdlib>
using namespace std;
using namespace robgp;

Problem::~Problem()
{

}

ProblemSource::ProblemSource()
{
	infinite=true;	// With a generic problem source, we can't really tell if its finite or not
}

ProblemSource::~ProblemSource()
{

}

bool ProblemSource::newProblem(Problem* &prob)
{
	prob=NULL;	// By default no problem data
	managed=false;	// By default, these problems will not free themselves
	return true;
}

ProblemInstance* Problem::createInstance()
{
	return NULL;	// By default no mutated problem data
}



void ListProblemSource::addProblem(Problem* prob)
{
	problems.push_back(prob);
}

bool ListProblemSource::newProblem(Problem* &prob)
{
	bool isnew=true;
	i++;
	if (i==problems.size()){
		i=0;
		isnew=false;
	}
	prob=problems[i];
	return isnew; // If i is 0 then you have received a recycled problem
}

ListProblemSource::ListProblemSource()
{
	infinite=false;
	managed=true;	// The list will be deleted by this class, hence selected problems should not be freed
	i=-1;
}

ListProblemSource::~ListProblemSource()
{
	for (int i=0;i<problems.size();++i)
		delete problems[i];
}


