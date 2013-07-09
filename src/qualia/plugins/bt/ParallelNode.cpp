#include "BehaviorTree.h"
#include <iostream>
using namespace BehaviorTree;
using namespace std;

ParallelNode::ParallelNode(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy) : childrenStatus(0)
{
	failPolicy = failurePolicy;
	succeedPolicy = successPolicy;
}

ParallelNode::~ParallelNode() {
  Alloc::free(childrenStatus);
}

void ParallelNode::init(void* agent)
{
  if (!childrenStatus)
    childrenStatus = (BEHAVIOR_STATUS*) Alloc::malloc(nChildren*sizeof(BEHAVIOR_STATUS));

  for (uint8_t i=0; i<nChildren; i++)
    children[i]->init(agent);

	for (uint8_t i = 0 ; i<nChildren; i++)
	  childrenStatus[i] = BT_RUNNING;
}

BEHAVIOR_STATUS ParallelNode::execute(void* agent)
{
//	if (childrenStatus == NULL)
//		init(agent);

	// go through all children and update the childrenStatus
	for (uint8_t i = 0 ; i<nChildren; i++)
	{
		BehaviorTreeNode* node = children[i];
		if (childrenStatus[i] == BT_RUNNING)
		{
			BEHAVIOR_STATUS status = node->execute(agent);
			if (status == BT_FAILURE)
			{
				if (failPolicy == FAIL_ON_ONE)
				{
					init(agent);
					return BT_FAILURE;
				}
				else
				{
					childrenStatus[i] = BT_FAILURE;
				}
			}
			if (status == BT_SUCCESS)
				childrenStatus[i] = BT_SUCCESS;
		}
		if (childrenStatus[i] == BT_FAILURE && failPolicy == FAIL_ON_ALL) //theoretically the failPolicy check is not needed
		{
			BEHAVIOR_STATUS status = node->execute(agent);
			childrenStatus[i] = status;
		}
	}

	//look through the childrenStatus and see if we have met any of our end conditions
	bool sawSuccess = false;
	bool sawAllFails = true;
	bool sawAllSuccess = true;
	for (uint8_t i = 0 ; i<nChildren; i++)
	{
		switch(childrenStatus[i])
		{
		case BT_SUCCESS:
			//can't instantly return success for succeedOnOne policy if failOnOne is also true, because failOnOne overrides succeedOnOne
			if (succeedPolicy == SUCCEED_ON_ONE && failPolicy != FAIL_ON_ONE)
			{
				init(agent);
				return BT_SUCCESS;
			}
			sawSuccess = true;
			sawAllFails = false;
			break;
		case BT_FAILURE:
			if (failPolicy == FAIL_ON_ONE)
			{
				init(agent);
				return BT_FAILURE;
			}
			sawAllSuccess = false;
			break;
		case BT_RUNNING:
			sawAllFails = false;
			sawAllSuccess = false;
			//optimization for early exit
			if (failPolicy == FAIL_ON_ALL && succeedPolicy == SUCCEED_ON_ALL)
			{
				return BT_RUNNING;
			}
			break;
		}
	}
	if (sawAllFails && failPolicy == FAIL_ON_ALL)
	{
		init(agent);
		return BT_FAILURE;
	}
	else if ( (sawAllSuccess && succeedPolicy == SUCCEED_ON_ALL) || (sawSuccess && succeedPolicy == SUCCEED_ON_ONE) )
	{
		init(agent);
		return BT_SUCCESS;
	}
	else
	{
		return BT_RUNNING;
	}
}
