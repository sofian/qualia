#include "BehaviorTree.h"

using namespace BehaviorTree;
using namespace std;

CountLimitNode::CountLimitNode(int _limit, bool _allow_reinitialize)
{
  limit = _limit;
  allow_reinitialize = _allow_reinitialize;
  current_rep = 0;
};

//
//BehaviorTreeNode* CountLimitNode::setChildren(BehaviorTreeNode* node, ...)
//{
//  va_list vl;
//  va_start(vl, node);
//  BehaviorTreeInternalNode::setChildren(node, vl);
//  va_end(vl);
//  Q_ASSERT_ERROR_MESSAGE(nChildren <= 1, "RepeatNode accepts maximum one (1) children node.");
//  return this;
//}

BEHAVIOR_STATUS CountLimitNode::execute(void* agent)
{
	if (current_rep == limit)
		return BT_FAILURE;
	if (nChildren == 0)
	{
		current_rep++;
		return BT_SUCCESS;
	}
	else
	{
		BEHAVIOR_STATUS status = children[0]->execute(agent);
		if (status == BT_SUCCESS || status == BT_FAILURE)
		{
			current_rep++; //only increment the count when we've finished a job
			initChildren(agent);
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
	initChildren(agent);
}

void CountLimitNode::initChildren(void* agent)
{
	if (nChildren == 1)
	  children[0]->init(agent);
}
