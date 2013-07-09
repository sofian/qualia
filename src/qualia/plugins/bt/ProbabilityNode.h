#include "mtrand.h"

namespace BehaviorTree
{

/**
 * This class is used solely to pass arguments in the setWeightedChildren() method.
 */
struct WeightedBehaviorTreeNode {
  float weight;
  BehaviorTreeNode* node;

  WeightedBehaviorTreeNode(float weight_, BehaviorTreeNode* node_) : weight(weight_), node(node_) {}

  bool isNull() const { return (node == NULL); }

  /// Null node, used to terminate argument list when calling setWeightedChildren().
  static const WeightedBehaviorTreeNode NULL_WEIGHTED_NODE;
};

///Executes behaviors randomly, based on a given set of weights.
/** The weights are not percentages, but rather simple ratios.
For example, if there were two children with a weight of one, each would have a 50% chance of being executed.
If another child with a weight of eight were added, the previous children would have a 10% chance of being executed, and the new child would have an 80% chance of being executed.
This weight system is intended to facilitate the fine-tuning of behaviors.
*/
class ProbabilityNode:public BehaviorTreeInternalNode
{
public:
  ProbabilityNode();
  virtual ~ProbabilityNode();

  /// Sets children, giving them all the same weight.
  virtual BehaviorTreeNode* setChildren(BehaviorTreeNode* node, ...);

  /**
   * Sets children with different weights. Accepts WeightedBehaviorTreeNode instead of BehaviorTreeNode.
   * List of arguments should be terminated by WeightedBehaviorTreeNode::NULL_WEIGHTED_NODE.
   */
  virtual BehaviorTreeNode* setWeightedChildren(WeightedBehaviorTreeNode weightedNode, ...);

  /**
   * Adds a child to the node with specific weight. Deprecated: use setWeightedChildren() method instead.
   * @deprecated
   */
#if is_computer()
  virtual ProbabilityNode* addChild(BehaviorTreeNode* node, float weight);
#endif

  void init(void* object);
	BEHAVIOR_STATUS execute(void* object);

private:
	MTRand_closed random;
	float totalSum;
	BehaviorTreeNode* currentNode;
	float* weighting;
};

}
