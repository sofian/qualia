#include "BehaviorTree.h"
#include <assert.h>
using namespace BehaviorTree;
using namespace std;

BEHAVIOR_STATUS RepeatNode::execute(void* agent)
{
	if (nChildren== 0)
		return BT_SUCCESS;
	else
	{
		BEHAVIOR_STATUS status = children[0]->execute(agent);
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
	if (nChildren == 1)
		children[0]->init(agent);
}

/** \param repeats The number of times to repeat. An argument of -1 indicates the node should repeat indefinitely. Other negative numbers have undefined behavior */
RepeatNode::RepeatNode(BehaviorTreeNode* child, int repeats)
  : BehaviorTreeInternalNode(&child, 1)
{
	target = repeats;
	count = 0;
};
