#include "BehaviorTree.h"
#include <assert.h>
using namespace BehaviorTree;
using namespace std;
BEHAVIOR_STATUS RepeatNode::execute(void* agent)
{
	if (children.size()== 0)
		return BT_SUCCESS;
	else
	{
		BEHAVIOR_STATUS status = children.at(0)->execute(agent);
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
	if (children.size() == 1)
		children.at(0)->init(agent);
}

BehaviorTreeInternalNode* BehaviorTree::RepeatNode::addChild( BehaviorTreeNode* newChild )
{
	Q_ASSERT_ERROR_MESSAGE(children.size() == 0, "RepeatNode can only contain one child.");
	BehaviorTreeInternalNode::addChild(newChild);
	return this;
}

/** \param repeats The number of times to repeat. An argument of -1 indicates the node should repeat indefinitely. Other negative numbers have undefined behavior */
RepeatNode::RepeatNode(int repeats)
{
	target = repeats;
	count = 0;
};
