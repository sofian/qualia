
#include "BehaviorTree.h"

using namespace BehaviorTree;

const WeightedBehaviorTreeNode WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE = WeightedBehaviorTreeNode(0, NULL);

ProbabilityNode::ProbabilityNode() : weighting(0)
{
  totalSum = 0;
  currentNode = NULL;
//  weighting = NULL;
  random.seed(millis());
}

ProbabilityNode::~ProbabilityNode() {
  Alloc::free(weighting);
}

BehaviorTreeNode* ProbabilityNode::setChildren(BehaviorTreeNode* node, ...) {
  va_list vl;
  va_start(vl, node);
  BehaviorTreeInternalNode::_setChildren(node, vl);
  va_end(vl);

  weighting = (float*)Alloc::malloc(nChildren*sizeof(float));
  for (uint8_t i=0; i<nChildren; i++) {
    weighting[i] = 1;
    totalSum += weighting[i];
  }

  return this;
}

//BehaviorTreeNode* ProbabilityNode::setWeightedChildren(float weight, BehaviorTreeNode* node, ...) {
//  va_list vl;
//  va_start(vl, node);
//
//  nChildren = 0;
//  while (true) {
//    nChildren++;
//
//    weighting = (float*)Alloc::realloc(weighting, nChildren * sizeof(float));
//    children = (BehaviorTreeNode**) Alloc::realloc(children, nChildren * sizeof(BehaviorTreeNode*));
//
//    weighting[nChildren-1] = weight;
//    children[nChildren-1]  = node;
//
//    totalSum += weight;
//
//    weight = va_arg(vl, double);
//    if (weight <= 0) break;
//    node = va_arg(vl, BehaviorTreeNode*);
//  }
//  va_end(vl);
//
//  return this;
//}

BehaviorTreeNode* ProbabilityNode::setWeightedChildren(WeightedBehaviorTreeNode weightedNode, ...)
{
 va_list vl;
 va_start(vl, weightedNode);

 nChildren = 0;
 while (!weightedNode.isNull()) { // terminate on null (ie. WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE)
   nChildren++;

   children = (BehaviorTreeNode**) Alloc::realloc(children, nChildren * sizeof(BehaviorTreeNode*));
   weighting = (float*)Alloc::realloc(weighting, nChildren * sizeof(float));

   children[nChildren-1]  = weightedNode.node;
   weighting[nChildren-1] = weightedNode.weight;

   totalSum += weightedNode.weight;
   weightedNode = va_arg(vl, WeightedBehaviorTreeNode);
 }
 va_end(vl);

 return this;
}

#if is_computer()
ProbabilityNode* ProbabilityNode::addChild(BehaviorTreeNode* node, float weight) {
  nChildren++;
  weighting = (float*) Alloc::realloc(weighting, nChildren*sizeof(float));
  children = (BehaviorTreeNode**) Alloc::realloc(children, nChildren*sizeof(BehaviorTreeNode*));
  weighting[nChildren-1] = weight;
  children[nChildren-1] = node;
  return this;
}
#endif

void ProbabilityNode::init(void* agent)
{
	currentNode = NULL;
  for (uint8_t i=0; i<nChildren; i++)
    children[i]->init(agent);
}

BEHAVIOR_STATUS ProbabilityNode::execute(void* agent)
{
	//check if we've already chosen a node to run
	if (currentNode != NULL)
	{
		BEHAVIOR_STATUS status = currentNode->execute(agent);
		if (status != BT_RUNNING)
			currentNode = NULL;
		return status;
	}

	// TODO: Use Qualia random instead of the stuff provided here
	double chosen = random() * totalSum; //generate a number between 0 and the sum of the weights

	double sum = 0;
	for (uint8_t i=0; i<nChildren; i++)
	{
		sum += weighting[i];
		if (sum >= chosen) //execute this node
		{
			BEHAVIOR_STATUS status = children[i]->execute(agent);

			if (status == BT_RUNNING)
				currentNode = children[i];
			else
				currentNode = NULL;
			return status;
		}
	}

	Q_ERROR("This shouldn't happen");
	return BT_FAILURE;
	//throw new exception();//"shouldn't be here"); //theoretically fp error can put us here, but it's so uncommon that it should raise a flag if it ever happens
}

