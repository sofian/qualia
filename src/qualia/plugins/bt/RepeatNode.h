namespace BehaviorTree
{
///A node that repeats its child a specified number of times.
/** If the child returns BT_FAILURE, RepeatNode will also return BT_FAILURE. However, if the child returns BT_SUCCESS and it hasn't completed the specified number of repetitions, RepeatNode will restart it and continue returning BT_RUNNING. 
*/
class RepeatNode: public BehaviorTreeInternalNode
{
public:
	/** \param repeats the number of times to repeat the execution of the node's child before returning BT_SUCCESS
	*/
	RepeatNode::RepeatNode(int repeats);
	BehaviorTreeInternalNode* addChild(BehaviorTreeNode* newChild);
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
protected:
	int count;
	int target;

};
}