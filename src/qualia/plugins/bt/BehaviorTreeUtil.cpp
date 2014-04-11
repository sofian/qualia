#include "BehaviorTreeUtil.h"

namespace BehaviorTree {

PriorityNode*    BehaviorTreeUtil::priority(TRAVERSAL_POLICY traversalPolicy)
{ return Q_NEW(PriorityNode)(traversalPolicy); }

SequentialNode*  BehaviorTreeUtil::sequential()
{ return Q_NEW(SequentialNode)(); }

ParallelNode*    BehaviorTreeUtil::parallel(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy)
{ return Q_NEW(ParallelNode)(failurePolicy, successPolicy); }

ProbabilityNode* BehaviorTreeUtil::probability()
{ return Q_NEW(ProbabilityNode)(); }

RepeatNode*      BehaviorTreeUtil::repeat(int repeats)
{ return Q_NEW(RepeatNode)(repeats); }

CountLimitNode*  BehaviorTreeUtil::countLimit(int limit, bool allow_reinitialize)
{ return Q_NEW(CountLimitNode)(limit, allow_reinitialize); }

AlwaysRunning* BehaviorTreeUtil::running() {
  return Q_NEW(AlwaysRunning)();
}

AlwaysSuccess* BehaviorTreeUtil::success() {
  return Q_NEW(AlwaysSuccess)();
}

AlwaysFailure* BehaviorTreeUtil::failure() {
  return Q_NEW(AlwaysFailure)();
}

SuccessAfter* BehaviorTreeUtil::successAfter(int t) {
  return Q_NEW(SuccessAfter)(t);
}

FailureAfter* BehaviorTreeUtil::failureAfter(int t) {
  return Q_NEW(FailureAfter)(t);
}

WeightedBehaviorTreeNode BehaviorTreeUtil::weighted(double weight, BehaviorTreeNode* node) {
  return WeightedBehaviorTreeNode(weight, node);
}

BehaviorTreeNode* end() {
  return BT_END;
}

WeightedBehaviorTreeNode endWeighted() {
  return BT_END_WEIGHTED;
}


BehaviorTreeUtil BT;

}
