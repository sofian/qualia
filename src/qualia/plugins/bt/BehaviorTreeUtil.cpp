#include "BehaviorTreeUtil.h"

namespace BehaviorTree {

PriorityNode*    BT_PRIORITY()          { return Q_NEW(PriorityNode)(); }

SequentialNode*  BT_SEQUENTIAL()        { return Q_NEW(SequentialNode)(); }

ParallelNode*    BT_PARALLEL(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy)
  { return Q_NEW(ParallelNode)(failurePolicy, successPolicy); }

ProbabilityNode* BT_PROBABILITY()       { return Q_NEW(ProbabilityNode)(); }

RepeatNode*      BT_REPEAT(int repeats) { return Q_NEW(RepeatNode)(repeats); }

CountLimitNode*  BT_COUNT_LIMIT(int limit, bool allow_reinitialize)
  { return Q_NEW(CountLimitNode)(limit, allow_reinitialize); }

WeightedBehaviorTreeNode _WEIGHTED(double weight, BehaviorTreeNode* node) {
  return WeightedBehaviorTreeNode(weight, node);
}

}
