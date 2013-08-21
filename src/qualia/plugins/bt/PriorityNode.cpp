#include "BehaviorTree.h"

using namespace BehaviorTree;

PriorityNode::PriorityNode(TRAVERSAL_POLICY traversalPol)
{
	currentPosition = -1;
	traversalPolicy = traversalPol;
}

void PriorityNode::init(void* agent)
{
	currentPosition = -1;
	for (uint8_t i=0; i<nChildren; i++)
	  children[i]->init(agent);
}

BEHAVIOR_STATUS PriorityNode::execute(void* agent)
{
  if (currentPosition == -1) //starting out
  {
    init(agent);
    currentPosition = 0;
  }
  else if (traversalPolicy == RESTART) // restart on running
  {
    currentPosition = 0;
  }

  if (nChildren == 0)
    return BT_SUCCESS;

  BehaviorTreeNode* currentlyRunningNode = children[currentPosition];
  BEHAVIOR_STATUS status;
  while ((status = currentlyRunningNode->execute(agent)) == BT_FAILURE) //keep trying children until one doesn't fail
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
  if (status == BT_RUNNING)
    return BT_RUNNING;
  else
  {
    currentPosition = -1;
    return BT_SUCCESS;
  }
}
