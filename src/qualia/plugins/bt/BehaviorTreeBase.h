#include <vector>
#include <map>

namespace BehaviorTree
{
	/// Enumerates the states every node can be in after execution during a particular time step
	/**
	- BT_SUCCESS indicates the node has completed running during this time step.
	- BT_FAILURE indicates the node has determined it will not be able to complete its task.
	- BT_RUNNING indicates that the node has successfully moved forward during this time step, but the task is not yet complete.
	*/
	enum BEHAVIOR_STATUS {BT_SUCCESS,BT_FAILURE,BT_RUNNING};
	class BehaviorTreeNode;

	/// Enumerates the options for when a parallel node is considered to have failed.
	/**
	- FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
	- FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

	If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
	*/
	enum FAILURE_POLICY {FAIL_ON_ONE,FAIL_ON_ALL};
	/// Enumerates the options for when a parallel node is considered to have succeeded.
	/**
	- SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
	- SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
	*/
	enum SUCCESS_POLICY {SUCCEED_ON_ONE,SUCCEED_ON_ALL};


	///Abstract base clase for Behavior Tree Nodes
	class BehaviorTreeNode
	{
	public:
		/// This method is invoked by the node's parent when the node should be run.
		virtual BEHAVIOR_STATUS execute(void* agent) = 0;
		/// This method will be invoked before the node is executed for the first time.
		virtual void init(void* agent) = 0;

		virtual ~BehaviorTreeNode(){}
	};

	/// Abstract base class for Behavior Tree nodes with children
	class BehaviorTreeInternalNode:public BehaviorTreeNode
	{

	public:
	  BehaviorTreeInternalNode(BehaviorTreeNode** children_, uint8_t nChildren_) : nChildren(nChildren_) {
	    children = (BehaviorTreeNode**) Alloc::malloc(nChildren * sizeof(BehaviorTreeNode*));
	    memcpy(children, children_, nChildren*sizeof(BehaviorTreeNode*));
	  }
		virtual BEHAVIOR_STATUS execute(void* agent) = 0;
		virtual void init(void* object) = 0;
    BehaviorTreeInternalNode* addChild( BehaviorTreeNode* newChild );

    virtual ~BehaviorTreeInternalNode()
    {
      for (uint8_t i=0; i<nChildren; i++)
        Alloc::free(children[i]);
    }
  public: // used to be protected
    BehaviorTreeNode** children;
    uint8_t nChildren;
	};

	///Always returns the BT_RUNNING status
	class AlwaysRunning: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_RUNNING;
		}
		void init(void* agent){};
	};

	///Always returns the BT_SUCCESS status
	class AlwaysSuccess: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_SUCCESS;
		}
		void init(void* agent){};
	};

	///Always returns the BT_FAILURE status
	class AlwaysFailure: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_FAILURE;
		}
		void init(void* agent){};
	};

	///Returns BT_RUNNING for the specified number of iterations, then returns BT_SUCCESS after that
	class SuccessAfter: public BehaviorTreeNode
	{
	public:
		int n;
		int total;
		BEHAVIOR_STATUS execute(void* agent)
		{
			if (n == 0)
			{
				return BT_SUCCESS;
			}
			else
			{
				n--;
				return BT_RUNNING;
			}
		}
		void init(void* agent)
		{
			n = total;
		};
		SuccessAfter(int t)
		{
			total = t;
			n = total;
		}
	};

	///Returns BT_RUNNING for the specified number of iterations, then returns BT_FAILURE after that
	class FailureAfter: public BehaviorTreeNode
	{
	public:
		int n;
		int total;
		BEHAVIOR_STATUS execute(void* agent)
		{
			if (n == 0)
			{
				return BT_FAILURE;
			}
			else
			{
				n--;
				return BT_RUNNING;
			}
		}
		void init(void* agent)
		{
			n = total;
		};
		FailureAfter(int t)
		{
			total = t;
			n = total;
		}
	};
}
