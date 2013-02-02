#include "Program.hpp"
#include <stdexcept>
#include <sstream>
#include "System.hpp"
using namespace std;
using namespace robgp;

Program::Program(System* system)
{
	this->system=system;
}

Program::~Program()
{
}

#ifdef DEBUG
bool Program::checkConsistency()
{
	if (functions.size()+1!=branch.size())
	{
		throw runtime_error("Number of functions doesn't match number of branches");
	}
	for (int i=0;i<branch.size();++i)
	{
		if (i>0)
		{
			ADF* curfunc=(ADF*)functions[i-1];
			if (curfunc->adf_no!=i-1)
				throw runtime_error("ADF number does not match");
			if (curfunc->arg_type.size()-1!=curfunc->args.size())
				throw runtime_error("Number of arguments does not match function specification");
			for (int j=0;j<curfunc->args.size();++j)
			{
				if (((ADFarg*)curfunc->args[j])->argNo!=j)
					throw runtime_error("Argument number does not match argument position");
			}
		}else{
			if (system->returntype!=branch[0].front().f->arg_type[0])
				throw runtime_error("Root node return type should be "+system->typestr(system->returntype)+" but is "+system->typestr(branch[0].front().f->arg_type[0]));
		}
		if (!TreeNodeIter(&branch[i], 0).checkConsistency(system, &functions, i))
			throw runtime_error("Branch is not consistent");
	}
	return true;
}

string Program::to_string()
{
	stringstream ss;
	ss << *this;
	return ss.str();
}

#endif

Datatype Program::eval(Problem* prob, ProblemInstance* inst)
{
//	cerr << "Executing "<< (*this) << endl;
#ifdef DEBUG
	if (!checkConsistency())
		throw runtime_error("Inconsistent program");
//	cerr << "Evaluating " << *this;
#endif
	TreeNodeIter it=TreeNodeIter(&branch[0],0);
	Frame f=Frame(this, system->recursiveDepth);
	f.prob=prob;
	f.inst=inst;
	return it->f->eval(it, &f);
}

void Program::copy(Program& result)
{
	result.functions.assign(functions.begin(), functions.end());
	result.branch.clear();
	for (int i=0;i<branch.size();++i)
	{
		result.branch.push_back(vector<TreeNode>());
		result.branch[i].assign(branch[i].begin(), branch[i].end());
	}
}

ostream& operator<< (ostream& os, Program& prgm)
{
	if (prgm.branch.size()==0)
		os << "No program branches";
	else if (prgm.branch[0].size()==0)
		os << "No main branch";
	else{
		os << prgm.system->typestr(prgm.system->returntype) << " main(): ";
		TreeNodeIter it=TreeNodeIter(&prgm.branch[0],0);
		os << it << endl;

		for (int i=1;i<prgm.branch.size();++i)
		{
			it=TreeNodeIter(&prgm.branch[i], 0);
			os << prgm.system->typestr(prgm.functions[i-1]->arg_type[0]) << " ";
			os << prgm.functions[i-1]->name << "(";
			for (int j=1;j<prgm.functions[i-1]->arg_type.size();++j)
			{
				if (j>1)
					os <<", ";
				os << prgm.system->typestr(prgm.functions[i-1]->arg_type[j]);
			}
			os << "): ";
			os << it;
			os <<endl;
		}
	}
	return os;
}

bool Program::operator< (const Program& other) const
{
	return fitness[0].weightedstd < other.fitness[0].weightedstd;
}


