#include "BehaviorTree.h"

using namespace BehaviorTree;

CountLimitNode::CountLimitNode(int _limit, bool _allow_reinitialize)
{
  limit = _limit;
  allow_reinitialize = _allow_reinitialize;
  current_rep = 0;
};

BEHAVIOR_STATUS CountLimitNode::execute(void* agent)
{
	if (current_rep == limit)
		return BT_FAILURE;
	if (child == 0)
	{
		current_rep++;
		return BT_SUCCESS;
	}
	else
	{
		BEHAVIOR_STATUS status = child->execute(agent);
		if (status == BT_SUCCESS || status == BT_FAILURE)
		{
			current_rep++; //only increment the count when we've finished a job
			initChild(agent);
		}
		return status;
	}
}

void CountLimitNode::init( void* agent )
{
	if (allow_reinitialize)
	{
		current_rep = 0;
	}
	initChild(agent);
}

void CountLimitNode::initChild(void* agent)
{
	if (child)
	  child->init(agent);
}
