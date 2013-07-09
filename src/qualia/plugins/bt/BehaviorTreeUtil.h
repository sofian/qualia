#ifndef BEHAVIOR_TREE_UTIL_H_
#define BEHAVIOR_TREE_UTIL_H_

#include "BehaviorTree.h"

namespace BehaviorTree {

PriorityNode*    BT_PRIORITY();

SequentialNode*  BT_SEQUENTIAL();

ParallelNode*    BT_PARALLEL(FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY successPolicy = SUCCEED_ON_ALL);

ProbabilityNode* BT_PROBABILITY();

RepeatNode*      BT_REPEAT(int repeats);

CountLimitNode*  BT_COUNT_LIMIT(int limit, bool allow_reinitialize = true);

#define CHILDREN(...) setChildren(__VA_ARGS__, NULL)

#define WEIGHTED_CHILDREN(...) setWeightedChildren(__VA_ARGS__, WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE)

WeightedBehaviorTreeNode _WEIGHTED(double weight, BehaviorTreeNode* node);

}

#endif
