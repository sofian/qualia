#pragma once
#include "../BreedPopOperator.hpp"
#include "../SelectionMethod.hpp"
#include "../TreeGrower.hpp"

namespace robgp
{

namespace PopOperations
{

class ConfidenceTest : public BreedPopOperator
{
public:
	ConfidenceTest();
	~ConfidenceTest();
	
	virtual void initialize(System* system);
	virtual BreedPopOperator* clone();
	virtual void operate(System* system, Population* pop);
	
	vector<double> mean;
	vector<double> stddev;
protected:
private:


};

}};

