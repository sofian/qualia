#include "BehaviorTree.h"
using namespace BehaviorTree;
using namespace std;

PriorityNode::PriorityNode()
{
	currentPosition = -1;
}

void PriorityNode::init(void* agent)
{
	currentPosition = -1;
	for (uint8_t i=0; i<nChildren; i++)
	  children[i]->init(agent);
}

BEHAVIOR_STATUS PriorityNode::execute(void* agent)
{
	if (currentPosition != -1) //there's one still running
	{
		BEHAVIOR_STATUS status = (children[currentPosition])->execute(agent);
		if (status == BT_RUNNING)
			return BT_RUNNING;
		else if (status == BT_SUCCESS)
		{
			currentPosition = -1;
			return BT_SUCCESS;
		}
		else if (status == BT_FAILURE)
		{
			currentPosition++;
			if (currentPosition == (int)nChildren)
			{
				currentPosition = -1;
				return BT_FAILURE;
			}
		}
	}
	else
	{
		init(agent);
		currentPosition = 0;
	}
	if (nChildren == 0)
		return BT_SUCCESS;

	BehaviorTreeNode* currentlyRunningNode = children[currentPosition];
	BEHAVIOR_STATUS status;
	while ((status = (*currentlyRunningNode).execute(agent)) == BT_FAILURE) //keep trying children until one doesn't fail
	{
		currentPosition++;
		if (currentPosition == (int)nChildren) //all of the children failed
		{
			currentPosition = -1;
			return BT_FAILURE;
		}
		else
		  currentlyRunningNode = children[currentPosition];
	}
	return status;

}
