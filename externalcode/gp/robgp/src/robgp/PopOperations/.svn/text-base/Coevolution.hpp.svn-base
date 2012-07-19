#pragma once
#include "../BreedPopOperator.hpp"
#include "../SelectionMethod.hpp"
#include "../TreeGrower.hpp"
#include "../Evaluator.hpp"

namespace robgp
{

class CoevolutionEvaluator
{
public:
	CoevolutionEvaluator();
	virtual ~CoevolutionEvaluator();

	virtual void evaluate(vector<Program*> &prgm, Problem* prob=NULL)=0;

};

namespace PopOperations
{

class Coevolution : public BreedPopOperator
{
public:
	Coevolution();
	virtual ~Coevolution();
	
	virtual BreedPopOperator* clone();
	virtual void operate(System* system, Population* pop);
	virtual void initialize(System* system);

protected:
private:


};

}};

