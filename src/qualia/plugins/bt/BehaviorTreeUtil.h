#ifndef BEHAVIOR_TREE_UTIL_H_
#define BEHAVIOR_TREE_UTIL_H_

#include "BehaviorTree.h"

namespace BehaviorTree {

class BehaviorTreeUtil {
public:
  PriorityNode*    priority(TRAVERSAL_POLICY traversalPolicy = RESTART);

  SequentialNode*  sequential();

  ParallelNode*    parallel(FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY successPolicy = SUCCEED_ON_ALL);

  ProbabilityNode* probability();

  RepeatNode*      repeat(int repeats);

  CountLimitNode*  countLimit(int limit, bool allow_reinitialize = true);

  AlwaysRunning*   running();
  AlwaysSuccess*   success();
  AlwaysFailure*   failure();

  SuccessAfter*    successAfter(int t);
  FailureAfter*    failureAfter(int t);

  WeightedBehaviorTreeNode weighted(double weight, BehaviorTreeNode* node);

  BehaviorTreeNode* end();
  WeightedBehaviorTreeNode endWeighted();
};

#define BT_END NULL
#define BT_END_WEIGHTED WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE

extern BehaviorTreeUtil BT;

#define CHILDREN(...)          setChildren(__VA_ARGS__, NULL)
#define WEIGHTED_CHILDREN(...) setWeightedChildren(__VA_ARGS__, WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE)

}

#endif
