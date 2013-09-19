#include <qualia/core/common.h>
#include <stdarg.h>

#ifndef BEHAVIOR_TREE_BASE_H_
#define BEHAVIOR_TREE_BASE_H_

namespace BehaviorTree
{
	/// Enumerates the states every node can be in after execution during a particular time step
	/**
	- BT_SUCCESS indicates the node has completed running during this time step.
	- BT_FAILURE indicates the node has determined it will not be able to complete its task.
	- BT_RUNNING indicates that the node has successfully moved forward during this time step, but the task is not yet complete.
	*/
	enum BEHAVIOR_STATUS {BT_SUCCESS,BT_FAILURE,BT_RUNNING};

		/// Enumerates the options for when a parallel node is considered to have failed.
	/**
	- FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
	- FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

	If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
	*/
	typedef uint8_t FAILURE_POLICY;
	static const FAILURE_POLICY FAIL_ON_ONE = 1;
	static const FAILURE_POLICY FAIL_ON_ALL = 255;

	/// Enumerates the options for when a parallel node is considered to have succeeded.
	/**
	- SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
	- SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
	*/
  typedef uint8_t SUCCESS_POLICY;
  static const SUCCESS_POLICY SUCCEED_ON_ONE = 1;
  static const SUCCESS_POLICY SUCCEED_ON_ALL = 255;

	/// Enumerates the options for when a priority node returns BT_RUNNING.
	/**
	 - RESTART indicates that the priority node will restart from the beginning, thus ignoring the BT_RUNNING value
	 - CONTINUE indicates that the priority node will continue at the node that returned the BT_RUNNING value
	 */
	enum TRAVERSAL_POLICY {RESTART,CONTINUE};

	///Abstract base clase for Behavior Tree Nodes.
	class BehaviorTreeNode
	{
	public:
		/// This method is invoked by the node's parent when the node should be run.
		virtual BEHAVIOR_STATUS execute(void* agent) = 0;

		/// This method will be invoked before the node is executed for the first time.
		virtual void init(void* agent) = 0;

		virtual ~BehaviorTreeNode(){}
	};

	/// Abstract base class for Behavior Tree nodes with children.
	class BehaviorTreeInternalNode:public BehaviorTreeNode
	{
	public:
	  BehaviorTreeInternalNode();
    virtual ~BehaviorTreeInternalNode();

	  virtual BEHAVIOR_STATUS execute(void* agent) = 0;
		virtual void init(void* object) = 0;

		/**
		 * Sets the children nodes of this node. Adds children to node. The method will add all
		 * arguments (read as BehaviorTreeNode*) until NULL is reached.
		 *
		 * Example use:
		 * @code
		 * node->setChildren(child1, child2, child3, NULL);
		 * @endcode
		 */
		virtual BehaviorTreeNode* setChildren(BehaviorTreeNode* node, ...);

		/**
		 * Adds a child to the node. Deprecated: use setChildren() method instead.
		 * @deprecated
		 */
#if is_computer()
		virtual BehaviorTreeInternalNode* addChild(BehaviorTreeNode* node);
#endif

		/// Helper method for setChildren().
    virtual BehaviorTreeNode* _setChildren(BehaviorTreeNode* node, va_list nodeList);

    /// This node's children nodes.
    BehaviorTreeNode** children;

    /// The number of children of this node.
    uint8_t nChildren;
	};

  /// Abstract base class for Behavior Tree nodes with exactly one child.
  class BehaviorTreeDecoratorNode:public BehaviorTreeNode
  {
  public:
    BehaviorTreeDecoratorNode();
    virtual ~BehaviorTreeDecoratorNode();

    virtual BEHAVIOR_STATUS execute(void* agent) = 0;
    virtual void init(void* object) = 0;

    /// Sets the child node of this node.
    virtual BehaviorTreeNode* setChild(BehaviorTreeNode* node);

    /// This node's child node.
    BehaviorTreeNode* child;
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

#endif
