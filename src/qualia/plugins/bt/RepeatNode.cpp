#include "BehaviorTree.h"
#include <assert.h>
using namespace BehaviorTree;
using namespace std;

RepeatNode::RepeatNode(int repeats)
{
  target = repeats;
  count = 0;
};

//BehaviorTreeNode* RepeatNode::setChildren(BehaviorTreeNode* node, ...)
//{
//  va_list vl;
//  va_start(vl, node);
//  BehaviorTreeInternalNode::setChildren(node, vl);
//  va_end(vl);
//  Q_ASSERT_ERROR_MESSAGE(nChildren <= 1, "RepeatNode accepts maximum one (1) children node.");
//  return this;
//}

BEHAVIOR_STATUS RepeatNode::execute(void* agent)
{
	if (nChildren == 0)
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
