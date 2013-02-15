
#include "BehaviorTree.h"
using namespace BehaviorTree;
using namespace std;

#include <time.h>



void ProbabilityNode::init(void* agent)
{
	currentNode = NULL;
	for (BehaviorTreeListIter iter = children.begin();iter!=children.end();iter++)
		(*iter)->init(agent);
}
ProbabilityNode::ProbabilityNode()
{
	totalSum = 0;
	currentNode = NULL;
	random.seed((unsigned long)time(NULL));
}
BehaviorTreeInternalNode* ProbabilityNode::addChild(BehaviorTreeNode* node, double weighting)
{
	weightingMap[node] = weighting;
	totalSum += weighting;
	BehaviorTreeInternalNode::children.push_back(node);
	return this;
}

BehaviorTreeInternalNode* ProbabilityNode::addChild(BehaviorTreeNode* node)
{
	weightingMap[node] = 1;
	totalSum += 1;
	BehaviorTreeInternalNode::addChild(node);
	return this;
}

BEHAVIOR_STATUS ProbabilityNode::execute(void* agent)
{
	//check if we've already chosen a node to run
	if (currentNode != NULL)
	{
		BEHAVIOR_STATUS status = currentNode->execute(agent);
		if (status != BT_RUNNING)
			currentNode = NULL;
		return status;
	}

	double chosen = random() * totalSum; //generate a number between 0 and the sum of the weights

	double sum = 0;
	for (std::map<BehaviorTreeNode*,double>::iterator itr = weightingMap.begin() ; itr!=weightingMap.end() ; itr++)
	{
		sum += (*itr).second;
		if (sum >= chosen) //execute this node
		{
			BEHAVIOR_STATUS status = (*itr).first->execute(agent);

			if (status == BT_RUNNING)
				currentNode = itr->first;
			else
				currentNode = NULL;
			return status;
		}
	}



	throw new exception("shouldn't be here"); //theoretically fp error can put us here, but it's so uncommon that it should raise a flag if it ever happens
}
