namespace BehaviorTree
{
///Executes behaviors in priority order until one of them is successful.
/** Attempts to execute children in the order they were added. 
- If a child returns BT_FAILURE, begin executing the next child if there is one, else return BT_FAILURE.
- If a child returns BT_ERROR, return BT_ERROR.
- If a child returns BT_SUCCESS, return BT_SUCCESS.
- If a child returns BT_RUNNING, return BT_RUNNING.
*/
class PriorityNode:public BehaviorTreeInternalNode
{
public:
	BEHAVIOR_STATUS execute(void* agent);
	PriorityNode(TRAVERSAL_POLICY traversalPolicy=RESTART);
	void init(void* agent);

  int currentPosition;
  TRAVERSAL_POLICY traversalPolicy;
};

}
