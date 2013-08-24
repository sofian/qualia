namespace BehaviorTree
{
///A node that repeats its child a specified number of times.
/** If the child returns BT_FAILURE, RepeatNode will also return BT_FAILURE. However, if the child returns BT_SUCCESS and it hasn't completed the specified number of repetitions, RepeatNode will restart it and continue returning BT_RUNNING. 
*/
class RepeatNode: public BehaviorTreeDecoratorNode
{
public:
  /** \param repeats The number of times to repeat. An argument of -1 indicates the node should repeat indefinitely. Other negative numbers have undefined behavior */
	RepeatNode(int repeats);

	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);

	int count;
	int target;
};

}
