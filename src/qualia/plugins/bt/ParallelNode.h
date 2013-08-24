namespace BehaviorTree
{
///Execute behaviors in parallel
/** There are two policies that control the flow of execution. The first is the policy for failure, and the second is the policy for success.
For failure, the options are "fail when one child fails" and "fail when all children fail".
For success, the options are similarly "complete when one child completes", and "complete when all children complete".
*/
class ParallelNode:public BehaviorTreeInternalNode
{
public:
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);

	/** \param failurePolicy Determines how many of the node's children must fail before the node fails
	   \param successPolicy Determines how many of the node's children must succeed before the node succeeds */
	ParallelNode(FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY successPolicy = SUCCEED_ON_ALL);
	virtual ~ParallelNode();

private:
	bool *childrenRunning;

	FAILURE_POLICY failPolicy;
	SUCCESS_POLICY succeedPolicy;
  uint8_t nFailure;
  uint8_t nSuccess;
};
}
