namespace BehaviorTree
{
///Executes behaviors in order
/** Executes its children in the order they were added.
If the currently executing child returns BT_FAILURE, BT_ERROR, or BT_RUNNING, this returns the same status.
If the currently executing child returns BT_SUCCESS, this begins executing the next child, if it exists. It continues in this fashion until one of the children returns BT_FAILURE, BT_ERROR, or BT_RUNNING. If all children have successfully executed, it will return BT_SUCCESS.
*/
class SequentialNode:public BehaviorTreeInternalNode
{
public:
	/// Construct a sequentialNode.
	SequentialNode();

	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);

	/// Where the SequentialNode currently is executing. -1 indicates the SequentialNode is not executing any child.
	int currentPosition;
};
}
