#include "TreeNode.hpp"
#include "Function.hpp"
#include "Frame.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <typeinfo>
#include <sstream>
using namespace robgp;
using namespace std;

TreeNode::TreeNode(Function* func)
{
	nodes=0;
	depth=0;
	parentOffset=-1;
	f=func;
}

TreeNode::~TreeNode()
{

}

TreeNodeIter::TreeNodeIter(vector<TreeNode>* t, int n)
{
	tree=t;
	node=n;
}

TreeNodeIter::~TreeNodeIter()
{

}

bool TreeNodeIter::operator!= (TreeNodeIter other)
{
	return !(tree==other.tree && node==other.node);
}

TreeNode& TreeNodeIter::operator* ()
{
	return tree->at(node);
}

TreeNode* TreeNodeIter::operator-> ()
{
	return &(tree->at(node));
}

TreeNodeIter& TreeNodeIter::operator++()
{
	node+=tree->at(node).nodes;
	return *this;
}

TreeNodeIter TreeNodeIter::begin()
{
	return TreeNodeIter(tree, node+1);
}

TreeNodeIter TreeNodeIter::end()
{
	if (node==-1 || tree->size()==0)
		return TreeNodeIter(tree, 0);
	return TreeNodeIter(tree, node+tree->at(node).nodes);
}

vector<TreeNode>::iterator TreeNodeIter::iter()
{
	return tree->begin() + node;
}

TreeNodeIter TreeNodeIter::parentNode()
{
	return TreeNodeIter(tree, node + tree->at(node).parentOffset);
}

void offsetNodeCount(TreeNodeIter node, int diff)
{
	// Now traverse up the tree, and adjust node counts

	while (node.node>=0)
	{
		TreeNodeIter parent=node.parentNode();

		if (parent.node>=0)
		{
			parent->depth=node->depth+1;
			parent->nodes+=diff;
			// check earlier trees for depths
			for (TreeNodeIter cnode=parent.begin();cnode!=node;++cnode)
				if (cnode->depth+1>parent->depth)
					parent->depth=cnode->depth+1;
			// The offset to the parent of each following sibling must be changed by diff
			TreeNodeIter lastchild=parent.end();	// Calculate the location of the following tree (just past the last sibling)
			++node;
			while (node!=lastchild)
			{
				node->parentOffset-=diff; //(parent.node-node.node);
				if (node->depth+1 > parent->depth)
					parent->depth=node->depth+1;
				++node;
			}
		}
		node=parent;
	}

}

TreeNodeIter TreeNodeIter::appendChild(TreeNode childnode)
{
	TreeNodeIter cnode=end();	// Get position for new child
	childnode.nodes=1;
	childnode.depth=0;
	if (cnode.node==0)
		childnode.parentOffset=-1;
	else
		childnode.parentOffset=node - cnode.node;
	if (cnode.node==tree->size())
		tree->push_back(childnode);
	else
		tree->insert(cnode.iter(), childnode);
	offsetNodeCount(cnode, 1);	// Offset node counts by 1
	return cnode;
}

#ifdef DEBUG
bool TreeNodeIter::checkConsistency(System* system, vector<Function*>* functions, int branch)
{
#ifdef DEBUG_FUNCTIONS
	stringstream funcstr;
#endif
	if (node<0 || node >=tree->size())
		throw runtime_error("Tree is empty or pointer damaged!");
	TreeNodeIter cnode=begin();
	TreeNodeIter lnode=end();
	if (cnode.node<=node || lnode.node<=node)
		throw runtime_error("Children nodes have to be after current node!");
	int childcount=0;
	if (lnode.node>tree->size())
		throw runtime_error("Subtree size is assumed bigger than tree");

	// Verify function integrity, function pointer should be in list of available functions
	bool found=false;
	Function* func=tree->at(node).f;
	for (int i=0;!found && i<system->functions.size();++i)
	{
		for (int j=0;j<system->functions[i].size();++j)
		{
#ifdef DEBUG_FUNCTIONS
			funcstr << system->functions[i][j]->name << " ("<<system->functions[i][j]<<"), ";
#endif
			if (func==system->functions[i][j])
			{
				found=true;
				break;
			}
		}
	}
	int maxbr=0;
	if (branch==0)
		maxbr=functions->size();
	else if (system->recursiveDepth>0)
		maxbr=functions->size();
	else
		maxbr=branch;
	for (int i=0;!found && i<maxbr;++i)
	{
#ifdef DEBUG_FUNCTIONS
		funcstr << "adf"<<i<<":"<< functions->at(i)->name << " ("<<functions->at(i)<<"), ";
#endif
		if (func==functions->at(i))
		{
			found=true;
			break;
		}
	}
	if (!found && branch>0)
	{
		ADF* curfunc=(ADF*)functions->at(branch-1);
		for (int i=0;i<curfunc->args.size();++i)
		{
#ifdef DEBUG_FUNCTIONS
			funcstr << "arg"<<i<<":"<< curfunc->args[i]->name << " ("<<curfunc->args[i]<<"), ";
#endif
			if (func==curfunc->args[i])
			{
				found=true;
				break;
			}
		}
	}
	
//	cerr << "["<<branch<<","<<node<<"]="<<f<<" ";
	if (!found)
	{
#ifdef DEBUG_FUNCTIONS
		cerr << "Call to unavailable function:" << endl;
		cerr << "Available functions at this point:" << endl << funcstr.str() << endl;
#endif
		throw runtime_error("Called function "+to_string(func)+" was not found in declared functions");
	}

	int nodecount=1;
	int truedepth=0;
	while (cnode.node < lnode.node)
	{
		childcount++;
		nodecount+=cnode.tree->at(cnode.node).nodes;
		if (cnode.tree->at(cnode.node).depth+1>truedepth)
			truedepth=cnode.tree->at(cnode.node).depth+1;
		if (!cnode.checkConsistency(system, functions, branch))
			return false;
		++cnode;
	}
	if (nodecount!=tree->at(node).nodes)
		throw runtime_error("Incorrect node count");
	if (truedepth!=tree->at(node).depth)
		throw runtime_error("Incorrect depth");
	if (cnode.node > lnode.node)
		throw runtime_error("Size of subtrees larger than size of parent tree");
	if (childcount != tree->at(node).arity)
		throw runtime_error("Arity does not match number of children");
	if (tree->at(node).arity < tree->at(node).f->min_args() || tree->at(node).arity > tree->at(node).f->max_args())
		throw runtime_error("Arity does not match function arity");
	return true;
}
#endif

void robgp::eraseTree(TreeNodeIter t)
{
	int diff=-(t->nodes);
	int poffset=t->parentOffset;
	int pnode=t.node + poffset;
	int pend;
	int rdepth=t->depth;

	t.tree->at(pnode).arity--;
	t.tree->erase(t.iter(), t.iter()+t->nodes);

	t.tree->at(pnode).nodes+=diff;
	pend=pnode+t.tree->at(pnode).nodes;
	while (t.node<pend)
	{
		t->parentOffset=pnode - t.node; // -=diff;
		++t;
	}
#ifdef DEBUG
	if (t.node!=pend)
		throw runtime_error("Did not end up squarely at the end of arguments for parent");
#endif

	if (t.tree->at(pnode).depth==rdepth+1)
	{
		// Need to recalculate depth
		TreeNodeIter parentnode=TreeNodeIter(t.tree, pnode);
		parentnode->depth=0;
		for (t=parentnode.begin();t!=parentnode.end();++t)
		{
			if (t->depth+1>parentnode->depth)
				parentnode->depth=t->depth+1;
		}
	}
	// Not sure why this was here: t->parentOffset = poffset;
	offsetNodeCount(TreeNodeIter(t.tree, pnode), diff);
}

void robgp::copyTree(TreeNodeIter dst, TreeNodeIter src, int paroffset)
{
	int removedSize=(paroffset!=0?0:dst->nodes);
	int addedSize=src->nodes;
	int diff=addedSize-removedSize;	// If the difference in tree sizes is non-zero than the parent pointers of all following children at each level will have to be corrected.
	
	int poffset=paroffset==0?dst->parentOffset:paroffset;
	if (paroffset==0)
	{
		dst.tree->erase(dst.iter(), dst.iter()+dst->nodes);
	}
	dst.tree->insert(dst.iter(), src.iter(), src.iter()+src->nodes);

	// Now traverse up the tree, and adjust node counts
	if (dst.node==0)
		dst->parentOffset=-1;
	else
		dst->parentOffset=poffset;
	int curnode=dst.node;
	offsetNodeCount(dst, diff);

}
void robgp::copyTree(TreeNodeIter dst, TreeNode t)
{
	int poffset = dst->parentOffset;
	int diff=1-dst->nodes;
	t.depth=0;
	t.nodes=1;
	t.parentOffset = poffset;
	if (diff<0)
		dst.tree->erase(dst.iter()+1, dst.iter()+dst->nodes);
	
	*dst = t;
	offsetNodeCount(dst, diff);
}

std::ostream& operator<< (std::ostream& os, robgp::TreeNodeIter& prgm)
{
	prgm->f->write(os, prgm);
	return os;
}


