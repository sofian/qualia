#include "Population.hpp"
#include <stdexcept>
#include <algorithm>
using namespace robgp;
using namespace std;

Population::Population(int psize)
{
	this->psize = psize;
}

Population::Population(Population* subpop, int groups)
{
	psize=subpop->psize;
	subpops.push_back(subpop);
	for (int i=1;i<groups;i++)
	{
		subpops.push_back(subpop->clone());
	}
}

void Population::setSize(int psize)
{
	this->psize=psize;
	if (subpops.size()>0)
	{
		int groupsize=psize/subpops.size();
		int extrasize=psize%subpops.size();

		for (int i=0;i<subpops.size();++i)
		{
			subpops[i]->setSize(groupsize+(i<extrasize?1:0));
		}
	}
}

Population* Population::clone()
{
	Population* clone=new Population(psize);
	for (int i=0;i<subpops.size();++i)
		clone->subpops.push_back(subpops[i]->clone());
	clone->sort();
	return clone;
}

void Population::unsort()
{
	if (subpops.size()>0)
	{
		// Straight out merge the programs of the subpops
		orderedprgms.clear();
		for (int i=0;i<subpops.size();++i)
		{
			subpops[i]->unsort();
			for (int j=0;j<subpops[i]->size();++j)
				orderedprgms.push_back((Program*)subpops[i]->at(j));
		}
	}
}

void Population::sort()
{
	if (subpops.size()==0)
		// Sort programs
		std::sort(programs.begin(), programs.end());
	else
	{
		// Do merge sort of subpopulations

		// First sort subpopulations
		for (int i=0;i<subpops.size();++i)
			subpops[i]->sort();

		int* nextp = new int[subpops.size()];
		for (int i=0;i<subpops.size();++i)
			nextp[i]=0;

		orderedprgms.clear();
		while (orderedprgms.size() < psize)
		{
			int best=0;
			for (int i=1;i<subpops.size();++i)
				if (nextp[best]>=subpops[best]->size() || (nextp[i]<subpops[i]->size() && *((Program*)(subpops[i]->at(nextp[i]))) < *((Program*)(subpops[best]->at(nextp[best])))))
					best = i;
			orderedprgms.push_back((Program*)subpops[best]->at(nextp[best]++));
		}

		delete [] nextp;
	}
}

void Population::copy(Population* dst)
{
	if (dst->subpops.size() != subpops.size())
		throw runtime_error("Trying to copy incompatible populations");
	for (int i=0;i<subpops.size();++i)
		subpops[i]->copy(dst->subpops[i]);
	if (dst->programs.size()>programs.size())
	{
		dst->programs.assign(programs.begin(), programs.end());
		dst->programs.push_back(programs.back());
	}else{
		dst->programs.assign(programs.begin(), programs.begin()+dst->programs.size());
	}
	dst->sort();
}

Population::~Population()
{
	for (int i=0;i<subpops.size();++i)
	{
		delete subpops[i];
	}
}

Selectable* Population::at(int index)
{
	if (index<programs.size())
		return &programs[index];
	
	if (index>=orderedprgms.size())
		throw runtime_error("Tried to select past end of population");
	return orderedprgms[index];
	
	/*index-=programs.size();
	int curpop=0;
	while (curpop<subpops.size() && index>=subpops[curpop]->psize)
	{
		index-=subpops[curpop]->psize;
		curpop++;
	}
	if (curpop>=subpops.size())
		throw runtime_error("Tried to select past end of population");
	return subpops[curpop]->at(index);*/
}

int Population::size()
{
	return psize;
}


