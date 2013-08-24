#include "BehaviorTree.h"

using namespace BehaviorTree;

RepeatNode::RepeatNode(int repeats)
{
  target = repeats;
  count = 0;
};

BEHAVIOR_STATUS RepeatNode::execute(void* agent)
{
	if (child == 0)
		return BT_SUCCESS;
	else
	{
		BEHAVIOR_STATUS status = child->execute(agent);
		if (status == BT_SUCCESS)
		{
			count++;
			if (count == target && target!= -1)
			{
				init(agent);
				return BT_SUCCESS;
			}
			else
				return BT_RUNNING;
		}
		return status;
	}
}

void BehaviorTree::RepeatNode::init( void* agent )
{
	count = 0;
	if (child)
	  child->init(agent);
}
