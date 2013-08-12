#ifndef SIMPLE_BEHAVIOR_TREE_AGENT_H_
#define SIMPLE_BEHAVIOR_TREE_AGENT_H_

#include <qualia/plugins/bt/BehaviorTreeAgent.h>

using namespace BehaviorTree;

class SimpleBehaviorTreeAgent : public BehaviorTreeAgent {
public:
  SimpleBehaviorTreeAgent(ActionProperties* actionProperties, BehaviorTreeNode* root)
    : BehaviorTreeAgent(actionProperties, root) {}
  virtual ~SimpleBehaviorTreeAgent() {}

  float getInfluenceX() const { return currentObservation->observations[0]; }
  float getInfluenceY() const { return currentObservation->observations[1]; }
  float getInfluenceNorm() const { return sqrt( pow(getInfluenceX(), 2) + pow(getInfluenceY(), 2)); }
  void moveTowardsInfluence(void*) {
    currentAction[0] = getInfluenceX() < 0 ? 0 : currentAction.nActions(0) - 1;
    currentAction[1] = getInfluenceY() < 0 ? 0 : currentAction.nActions(1) - 1;
  }
};

class ChooseAction : public BehaviorTreeNode
{
public:
  Action action;
  ChooseAction(ActionProperties* props, const action_dim_t* actions) : action(props) {
    for (int i=0; i<props->dim(); i++)
      action[i] = actions[i];
  }
  virtual ~ChooseAction() {}

  BehaviorTree::BEHAVIOR_STATUS execute(void* agent) {
    ((BehaviorTreeAgent*)agent)->getCurrentAction().copyFrom(action);
    return BT_SUCCESS;
  }
  virtual void init(void* agent) {}
};

class SetActionDim : public BehaviorTreeNode
{
public:
  int actionIdx;
  action_dim_t actionValue;
  SetActionDim(int actionIdx_, action_dim_t actionValue_) : actionIdx(actionIdx_), actionValue(actionValue_) {}
  virtual ~SetActionDim() {}
  BehaviorTree::BEHAVIOR_STATUS execute(void* agent) {
    ((BehaviorTreeAgent*)agent)->getCurrentAction()[actionIdx] = actionValue;
    return BT_SUCCESS;
  }
  virtual void init(void* agent) {}
};



#endif
