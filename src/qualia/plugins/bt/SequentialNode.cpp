#include "BehaviorTree.h"

using namespace BehaviorTree;

void SequentialNode::init(void* agent)
{
	currentPosition = -1;
  for (uint8_t i=0; i<nChildren; i++)
    children[i]->init(agent);
}

SequentialNode::SequentialNode()
{
	currentPosition = -1;
}


BEHAVIOR_STATUS SequentialNode::execute(void* agent)
{
	if (currentPosition == -1) //starting out
	{
		init(agent);
		currentPosition = 0;
	}

	if (nChildren == 0)
		return BT_SUCCESS;

	BehaviorTreeNode* currentTask = children[currentPosition];
	BEHAVIOR_STATUS result = currentTask->execute(agent);

	while(result == BT_SUCCESS)
	{
		if (currentPosition == (int)nChildren-1) //finished last task
		{
			currentPosition = -1; //indicate we are not running anything
			return BT_SUCCESS;
		}
		else
		{
			currentPosition++;
			currentTask = children[currentPosition];
			result = currentTask->execute(agent);
		}
	}
	if (result == BT_FAILURE)
		currentPosition = -1;
	return result;
}
