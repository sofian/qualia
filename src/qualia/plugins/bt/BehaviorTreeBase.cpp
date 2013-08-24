#include "BehaviorTreeBase.h"

namespace BehaviorTree
{

BehaviorTreeInternalNode::BehaviorTreeInternalNode()
  : children(0), nChildren(0) {
}

BehaviorTreeInternalNode::~BehaviorTreeInternalNode() {
  for (uint8_t i = 0; i < nChildren; i++)
    Q_DELETE(children[i]);
  Alloc::free(children);
}

BehaviorTreeNode* BehaviorTreeInternalNode::setChildren(BehaviorTreeNode* node, ...) {
  Q_ASSERT_ERROR_MESSAGE(children == NULL, "Children already assigned: cannot call setChildren() twice.");

  va_list vl;
  va_start(vl, node);
  _setChildren(node, vl);
  va_end(vl);
  return this;
}

BehaviorTreeNode* BehaviorTreeInternalNode::_setChildren(BehaviorTreeNode* node, va_list nodeList) {
  va_list vl;
  va_copy(vl, nodeList);
  nChildren = 0;
  while (node) {
    nChildren++;
    children = (BehaviorTreeNode**) Alloc::realloc(children, nChildren * sizeof(BehaviorTreeNode*));
    children[nChildren - 1] = node;
    node = va_arg(vl, BehaviorTreeNode*);
  }
  return this;
}

#if is_computer()
BehaviorTreeInternalNode* BehaviorTreeInternalNode::addChild(BehaviorTreeNode* node) {
  nChildren++;
  children = (BehaviorTreeNode**) Alloc::realloc(children, nChildren*sizeof(BehaviorTreeNode*));
  children[nChildren-1] = node;
  return this;
}
#endif

BehaviorTreeDecoratorNode::BehaviorTreeDecoratorNode() : child(0) { }

BehaviorTreeDecoratorNode::~BehaviorTreeDecoratorNode() {
  if (child)
    Q_DELETE(child);
}

BehaviorTreeNode* BehaviorTreeDecoratorNode::setChild(BehaviorTreeNode* node) {
  child = node;
}

}
