#include "BehaviorTree.h"

using namespace BehaviorTree;

ParallelNode::ParallelNode(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy)
  : childrenRunning(NULL), nFailure(0), nSuccess(0)
{
	failPolicy = failurePolicy;
	succeedPolicy = successPolicy;
}

ParallelNode::~ParallelNode() {
  Alloc::free(childrenRunning);
}

void ParallelNode::init(void* agent)
{
  if (!childrenRunning)
    childrenRunning = (bool*) Alloc::malloc(nChildren*sizeof(bool));

  for (uint8_t i=0; i<nChildren; i++)
    children[i]->init(agent);

	for (uint8_t i = 0 ; i<nChildren; i++)
	  childrenRunning[i] = false;

	nSuccess = 0;
	nFailure = 0;
}

BEHAVIOR_STATUS ParallelNode::execute(void* agent)
{
  // This means init() wasn't called...
	if (!childrenRunning)
		init(agent);

	// go through all children and update the childrenRunning
	// TODO: implement avec ceci: https://github.com/NetEase/pomelo-bt/blob/master/lib/node/parallel.js
	for (uint8_t i = 0 ; i<nChildren; i++)
	{
	  if (childrenRunning[i])
	  {
      BEHAVIOR_STATUS status = children[i]->execute(agent);
      childrenRunning[i] = (status == BT_RUNNING);

      if (status == BT_SUCCESS)
        nSuccess++;
      else if (status == BT_FAILURE)
        nFailure++;
	  }
	}

	if (nSuccess == nChildren || nSuccess >= succeedPolicy)
	{
	  init(agent);
	  return BT_SUCCESS;
	}
	else if (nFailure == nChildren || nFailure >= failPolicy)
	{
	  init(agent);
	  return BT_FAILURE;
	}
	else
	  return BT_RUNNING;
}
