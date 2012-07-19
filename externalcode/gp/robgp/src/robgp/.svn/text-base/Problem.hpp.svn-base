#pragma once
#include <vector>

namespace robgp
{
	using namespace std;


class ProblemInstance;
class Problem
{
public:
	virtual ~Problem();
	virtual ProblemInstance* createInstance();
};

class ProblemInstance
{
public:
	virtual ~ProblemInstance()
	{
	}
};

class ProblemSource
{
public:
	ProblemSource();
	virtual ~ProblemSource();
	virtual bool newProblem(Problem* &prob);
	bool infinite;
	bool managed;
};


class ListProblemSource : public ProblemSource
{
public:
	ListProblemSource();
	virtual ~ListProblemSource();
	void addProblem(Problem* prob);
	bool newProblem(Problem* &prob);
	
private:
	int i;
	vector<Problem*> problems;
};

};

