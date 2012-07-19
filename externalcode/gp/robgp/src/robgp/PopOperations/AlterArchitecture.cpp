#include "AlterArchitecture.hpp"
#include "../AlteredPopulation.hpp"
#include "../System.hpp"
#include <cstdlib>
#include <stdexcept>
#include <typeinfo>
#include "../Utility.hpp"
using namespace robgp;
using namespace robgp::PopOperations;

AlterArchitecture::AlterArchitecture() : BreedPopOperator()
{
	name = "alterarchitecture";
	grower=NULL;
	maxadfs = -1;
	maxargs = -1;
}

AlterArchitecture::~AlterArchitecture()
{
	if (grower)
		delete grower;
}

void AlterArchitecture::parseXMLNode(XmlReader& xml, System* system)
{
	if (xml.get_attribute("maxadfs")!="")
		maxadfs=atoi(xml.get_attribute("maxadfs").c_str());
	if (xml.get_attribute("maxargs")!="")
		maxargs=atoi(xml.get_attribute("maxargs").c_str());
}

void AlterArchitecture::parseXMLAttribute(XmlReader& xml, System* system)
{
	if (xml.get_name()=="grow")
	{
		for(int depth=xml.get_depth();xml.read() && xml.get_depth()>depth;)
		{
			if (xml.get_name()!="#text")
			{
				// Search for breed operator in system's list of breed operators
				BreedOperator* op=system->getBreedOperator(xml.get_name());
				if (op==NULL)
				{
					throw runtime_error("RobGP system does not have a breed operator loaded called "+xml.get_name()+" referenced at line "+to_string(xml.get_line_no()));
				}
				op=op->clone();
				op->parseXML(xml, system);
				if (grower==NULL)
				{
					grower=dynamic_cast<TreeGrower*>(op);
					if (grower==NULL)
						throw runtime_error("Passed operator "+(op->name)+" is not a tree growing operator");
				}else{
						cerr << "Warning, extra grow operator not being used" << endl;
				}

			}
		}
	}else
		BreedPopOperator::parseXMLAttribute(xml, system);
}

void AlterArchitecture::initialize(System* system)
{
	// Split population into correct number of groups
	affectedPop = system->pop = new AlteredPopulation(system->pop, groups);
}

TreeGrower* AlterArchitecture::getGrower()
{
	if (grower==NULL)
		throw runtime_error("No tree grower specified");
	return grower;
}

void AlterArchitecture::operate(System* system, Population* pop)
{
	AlteredPopulation* p=(AlteredPopulation*)pop;

	// First select a subpopulation to apply the operation to
	if (selection.size()==0)
		throw runtime_error("No selection method set for architecture altering operation");

	SelectableVector list;
	for (vector<Population*>::iterator it=pop->subpops.begin();it!=pop->subpops.end();++it)
		list.list.push_back(*it);

	Population* src=(Population*)selection[0]->select(&list, rand()%list.size(), NULL);
	Population* dst=(Population*)selection[1%selection.size()]->select(&list, rand()%list.size(), (Selectable*)src);

	// Determine indexes of chosen populations
	int srci, dsti;
	for (int i=0;i<list.list.size();++i)
	{
		if (src==list.list[i])
			srci=i;
		if (dst==list.list[i])
			dsti=i;
	}

	// Select one of the architecture altering operations
	int alterop;

	int argfuncs=0;	// Number of functions that have arguments - i.e. number that may delete
	int argxfuncs=0;	// Number of functions that can duplicate one more argument
	int argnfuncs=0;	// Number of functions that can have a new argument
	for (vector<Function*>::iterator it=p->functions[srci].begin();it!=p->functions[srci].end();++it)
	{
		argfuncs+=(*it)->arg_type.size()>1?1:0;

		// If the function has at least 1 argument, and it can have at least 1 more
		if ((*it)->arg_type.size()>1 && (maxargs<0 || (*it)->arg_type.size()-1<maxargs))
			argxfuncs++;	// This function can duplicate arguments
		
		if (maxargs<0 || (*it)->arg_type.size()-1<maxargs)
			argnfuncs++;
	}

	bool allowableops[6];
	int allowedops=0;
	allowableops[0]=(maxadfs<0 || p->functions[srci].size()<maxadfs);	// ADF creation
	allowableops[1]=(p->functions[srci].size()>0 && (maxadfs<0 || p->functions[srci].size()<maxadfs));	// ADF duplication
	allowableops[2]=(p->functions[srci].size()>0);	// ADF deletion
	allowableops[3]=argnfuncs>0;	// Argument creation
	allowableops[4]=argxfuncs>0;	// Argument duplication
	allowableops[5]=argfuncs>0;		// Argument deletion
	for (int i=0;i<6;++i)
		if (allowableops[i]) allowedops++;
	if (allowedops==0)
		return;

	// Randomly choose an allowed architecture altering operation
	allowedops=rand()%allowedops;
	for (alterop=0;alterop<6;alterop++)
	{
		if (allowableops[alterop] && allowedops--==0)
			break;
	}

	// Then there are several possible selections.
	int selfunc=-1;	// Selected function
	int selarg=-1;	// Selected argument

	// Select a function for duplication or deletion, or for argument creation
	if (alterop>=1 && alterop<=3)
	{
		if (alterop==3 && maxargs>=0)
		{
			// Select random function that can support adding an argument
			int viablefunc=rand()%argnfuncs;
			for (selfunc=0;selfunc<p->functions[srci].size();++selfunc)
				if (p->functions[srci][selfunc]->arg_type.size()-1<maxargs && viablefunc--==0)
					break;
		}else
			selfunc=rand()%(p->functions[srci].size());
	}
	else if (alterop==4)
	{
		// Select a function with arguments and an argument from that function
		int i=rand()%argxfuncs;

		// Find the i'th function with arguments
		for (selfunc=0;selfunc<p->functions[srci].size();++selfunc)
		{
			if (p->functions[srci][selfunc]->arg_type.size()>1 && (maxargs<0 || p->functions[srci][selfunc]->arg_type.size()-1<maxargs))
				if (i--==0)
					break;
		}

		// Choose an argument at random
		selarg=rand()%(p->functions[srci][selfunc]->arg_type.size()-1);
	}
	else if (alterop==5)
	{
		// Select a function with arguments and an argument from that function
		int i=rand()%argfuncs;

		// Find the i'th function with arguments
		for (selfunc=0;selfunc<p->functions[srci].size();++selfunc)
		{
			if (p->functions[srci][selfunc]->arg_type.size()>1)
				if (i--==0)
					break;
		}

		// Choose an argument at random
		selarg=rand()%(p->functions[srci][selfunc]->arg_type.size()-1);
	}


	if (dsti!=srci)
	{
		src->copy(dst);

		// While the above copy handles programs, functions must be copied manually
		//
		// Delete all old functions
		//
		for (int i=0;i<p->functions[dsti].size(); ++i)
		{
			delete p->functions[dsti][i];
		}
		p->functions[dsti].clear();

		// Now copy the other population's functions
		for (int i=0;i<p->functions[srci].size(); ++i)
		{
			p->functions[dsti].push_back(((ADF*)p->functions[srci][i])->clone());
		}

		// Finally, find all references to old functions and function arguments and replace with new references
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));

			// Change function list to represent new functions
			//prgm->functions.assign(system->adfs.begin(), system->adf.end());
			prgm->functions.erase(prgm->functions.begin()+prgm->functions.size()-p->functions[srci].size(), prgm->functions.end());
			prgm->functions.insert(prgm->functions.end(), p->functions[dsti].begin(), p->functions[dsti].end());

			// Operate
			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					for (int funci=0;funci<p->functions[srci].size();++funci)
					{
						ADF* oldfunc=(ADF*)p->functions[srci][funci];
						ADF* newfunc=(ADF*)p->functions[dsti][funci];

						if (prgm->branch[br][node].f==oldfunc)
							prgm->branch[br][node].f=newfunc;

						if (br>0 && prgm->functions[br-1]==newfunc)
						{
							for (int argi=0;argi<oldfunc->args.size();++argi)
							{
								ADFarg* oldarg=(ADFarg*)oldfunc->args[argi];
								ADFarg* newarg=(ADFarg*)newfunc->args[argi];

								if (prgm->branch[br][node].f==oldarg)
									prgm->branch[br][node].f=newarg;
							}
						}
					}
				}
			}
#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}
		
	}

	// Now the hard part, do the operation
//	cout << "Function operation "<<alterop << endl;
	if (alterop == 0)
	{
		// Choose a type by choosing a random non-terminal in the first program's main
		Program* prgm = (Program*)(dst->at(0));

		// Select random non-terminal node from main
		int nterms=0;
		int node=0;
		for (;node<prgm->branch[0].size();++node)
		{
			if (prgm->branch[0][node].arity>0)
			{
				nterms++;
			}
		}
		if (nterms==0)
		{
			// We have no choice, use a random terminal
			node = rand()%prgm->branch[0].size();
		}else{
			nterms = rand()%nterms;
			node=0;
			for (;node<prgm->branch[0].size();++node)
			{
				if (prgm->branch[0][node].arity>0)
				{
					
					if (nterms-- == 0)
						break;
				}
			}
		}

		// Create function entry with corresponding type
		vector<int> argtype;
		int functype=prgm->branch[0][node].f->arg_type[0];
		argtype.push_back(functype);
		ADF* newfunc=new ADF(argtype, "ADF"+to_string(system->adfs+p->functions[dsti].size()), system->adfs+p->functions[dsti].size());
		p->functions[dsti].push_back(newfunc);

		int br;
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));
			// Generate a branch for the new adf
			prgm->functions.push_back(newfunc);
			prgm->branch.push_back(vector<TreeNode>());

			// Count number of subtrees of corresponding type (with optional extra restrictions)
			nterms=0;
			// Has to copy from main because it won't have any arguments
			br = 0;
			for (int i=0;i<prgm->branch[br].size();++i)
			{
				if (prgm->branch[br][i].f->arg_type[0]==functype)
				{
					nterms++;
				}
			}

			if (nterms==0)
			{
				// Need to generate random ADF
				//p->prepareADFs(system, dsti, prgm->branch.size());
				getGrower()->growTree(system, TreeNodeIter(&prgm->branch.back(), 0), functype);
				//p->clearADFs(system, dsti);

				// Unfortunately there is nowhere to insert a call to this ADF.
			}else{
				nterms=rand()%nterms;

				br = 0;
				node=0;
				for (;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f->arg_type[0]==functype)
					{
						if (nterms-- == 0)
							break;
					}
				}

				// Copy subtree to ADF code
				copyTree(TreeNodeIter(&prgm->branch.back(), 0), TreeNodeIter(&prgm->branch[br], node), -1);

				// Replace subtree with call to ADF
				TreeNode t=p->functions[dsti].back()->createInstance();

				copyTree(TreeNodeIter(&prgm->branch[br], node), t);	// Copy ADF function in
			}
		#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}
	}
	else if (alterop == 1)
	{
		// Subroutine duplication
		ADF* oldfunc = (ADF*)p->functions[dsti][selfunc];
		ADF* newfunc = new ADF(p->functions[dsti][selfunc]->arg_type);
		int funcbr=1+system->adfs+selfunc+1;
		newfunc->adf_no = oldfunc->adf_no+1;
		newfunc->name = "ADF"+to_string(newfunc->adf_no);
		for (int i=selfunc+1;i<p->functions[dsti].size();++i)
		{
			((ADF*)(p->functions[dsti][i]))->adf_no++;	
			((ADF*)(p->functions[dsti][i]))->rename();	
		}
		p->functions[dsti].insert(p->functions[dsti].begin()+selfunc+1, newfunc);

		// Now search for calls to the duplicated function .. in each program in the dest pop
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));

			// Operate - first copy function branch to new one
			prgm->functions.insert(prgm->functions.begin()+system->adfs+selfunc+1, newfunc);
			prgm->branch.insert(prgm->branch.begin()+system->adfs + selfunc +2, vector<TreeNode>());
			prgm->branch[system->adfs + selfunc + 2].assign(prgm->branch[system->adfs+selfunc+1].begin(), prgm->branch[system->adfs+selfunc+1].end());
			// Within function branch, replace argument references with new ones
			for (int node=0;node<prgm->branch[funcbr].size();++node)
			{
				ADFarg* myarg=dynamic_cast<ADFarg*>(prgm->branch[funcbr][node].f);
				if (myarg!=NULL)
				{
					prgm->branch[funcbr][node].f=newfunc->args[myarg->argNo];
				}
			}
			
			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f==oldfunc)
					{
						// With 50% probability change it to the other function
						if (rand()%2)
						{
							prgm->branch[br][node].f=newfunc;
						}
					}
				}
				// Only need to look at branch 0 and then branch selfunc + 2 and beyond
				if (br==0)
					br=selfunc+system->adfs+2;	// Skip to end of new function branch
			}
#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}
	}
	else if (alterop == 2)
	{
		ADF* delfunc=static_cast<ADF*>(p->functions[dsti][selfunc]);

		vector<vector<TreeNode> > argcopy;	// Saved arguments
		vector<TreeNode> tmptree;
		vector<int> arglocs;	// Determine in advance (for time saving) the location of the arguments in the adf
		int funcbr = selfunc + system->adfs + 1;

		// Replace recursive function calls with random trees
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));

			for (int subnode=prgm->branch[funcbr].size()-1; subnode>=0; --subnode)
			{
				if (prgm->branch[funcbr][subnode].f==delfunc)
				{
//					cout << "Replacing recursive call to subnode "<<subnode<<endl;
					//recurselocs.push_back(subnode);
					getGrower()->growTree(system, TreeNodeIter(&tmptree, 0), delfunc->arg_type[0]);
					copyTree(TreeNodeIter(&(prgm->branch[funcbr]),subnode), TreeNodeIter(&tmptree,0));
					tmptree.clear();
					++subnode;
#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
				}
			}
#ifdef DEBUG
			for (int subnode=prgm->branch[funcbr].size()-1; subnode>=0; --subnode)
			{
				if (prgm->branch[funcbr][subnode].f==delfunc)
					throw runtime_error("Found recursive function call that should have been deleted already!");
			}
#endif
		}

		// delete the function entry
		for (int i=selfunc+1;i<p->functions[dsti].size();++i)
		{
			((ADF*)(p->functions[dsti][i]))->adf_no--;
			((ADF*)(p->functions[dsti][i]))->rename();
		}
		p->functions[dsti].erase(p->functions[dsti].begin()+selfunc);

		// Subroutine deletion
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));

			// Delete function
			prgm->functions.erase(prgm->functions.begin()+system->adfs+selfunc);
			arglocs.clear();

			// Find location of each argument in deleted subroutine
			for (int subnode=0; subnode<prgm->branch[funcbr].size(); ++subnode)
			{
				ADFarg* myarg=dynamic_cast<ADFarg*>(prgm->branch[funcbr][subnode].f);
				if (myarg!=NULL)
				{
					arglocs.push_back(subnode);
#ifdef DEBUG
	if (myarg->argNo>delfunc->args.size())
	{
		throw runtime_error("We found an invalid ADF argument number");
	}
#endif
				}
				
			}

			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f==delfunc)
					{
						// Copy arguments
						TreeNodeIter pnode=TreeNodeIter(&prgm->branch[br], node);
						for (TreeNodeIter cnode = pnode.begin(); cnode!=pnode.end(); ++cnode)
						{
							argcopy.push_back(vector<TreeNode>());
							copyTree(TreeNodeIter(&argcopy.back(),0), cnode, -1);
						}
#ifdef DEBUG
	if (argcopy.size()!=delfunc->args.size())
		throw runtime_error("Uhoh, we didn't copy enough arguments");
#endif

						// Then replace branch with function code
						copyTree(pnode, TreeNodeIter(&prgm->branch[funcbr],0));

						// Lastly replace all instances of arguments in the function with argument code
						for (int ait=arglocs.size()-1;ait>=0;--ait)
						{
							ADFarg* myarg=dynamic_cast<ADFarg*>(prgm->branch[br][node+arglocs[ait]].f);
							if (myarg==NULL)
								throw runtime_error("Node was supposed to be ADFarg but its not!");
							copyTree(TreeNodeIter(&prgm->branch[br], node+arglocs[ait]), TreeNodeIter(&argcopy[myarg->argNo],0));
						}


						argcopy.clear();
						// Can't skip subtree, because used arguments may have reference to the same function, in fact we may even have to recheck this node
						node--;

					}
				}
				// Only need to look at branch 0 and then branch selfunc + 2 and beyond
				if ((system->recursiveDepth<=0 && br==0) || (system->recursiveDepth>0 && br==funcbr-1))
					br=funcbr;	// Skip to end of deleted function branch
			}

			// Lastly, we can delete the subroutine branch
			prgm->branch.erase(prgm->branch.begin()+funcbr);
#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}

		delete delfunc;
	
	}
	else if (alterop==3)
	{
		// Argument creation
		
		// Choose a type by choosing a random non-terminal in the first program's subroutine
		Program* prgm = (Program*)(dst->at(0));
		ADF* modfunc = (ADF*)p->functions[dsti][selfunc];
		int funcbr=system->adfs + selfunc + 1;

		// Select random non-terminal node from the first program's subroutine
		int nterms=0;
		int node=0;
		for (;node<prgm->branch[funcbr].size();++node)
		{
			if (prgm->branch[funcbr][node].arity>0)
			{
				nterms++;
			}
		}
		if (nterms==0)
		{
			// We have no choice, use a random terminal
			node = rand()%prgm->branch[funcbr].size();
		}else{
			nterms = rand()%nterms;
			node=0;
			for (;node<prgm->branch[funcbr].size();++node)
			{
				if (prgm->branch[funcbr][node].arity>0)
				{
					
					if (nterms-- == 0)
						break;
				}
			}
		}

		// Now create an argument using the selected type
		int argtype = prgm->branch[funcbr][node].f->arg_type[0];
		p->functions[dsti][selfunc]->arg_type.push_back(argtype);
		ADFarg* newarg;
		((ADF*)(p->functions[dsti][selfunc]))->args.push_back(newarg=new ADFarg(((ADF*)(p->functions[dsti][selfunc]))->args.size(),argtype));

		// Now step through programs
		int br;
		vector<TreeNode> branchcode;
		TreeNodeIter copiedbranch=TreeNodeIter(&branchcode, 0);
		for (int pi=0;pi<dst->size();++pi)
		{
			branchcode.clear();
			Program* prgm = (Program*)(dst->at(pi));

			
			// Count number of subtrees of corresponding type (with optional extra restrictions)
			nterms=0;
			br = funcbr;
			for (int i=0;i<prgm->branch[br].size();++i)
			{
				if (prgm->branch[br][i].f->arg_type[0]==argtype)
				{
					nterms++;
				}
			}

			if (nterms==0)
			{
				// TODO: Need to generate random branch
	//			throw runtime_error("Generating a random argument branch is not yet handled, sorry");
				getGrower()->growTree(system, copiedbranch, argtype);
			}else{
				nterms=rand()%nterms;

				br = funcbr;
				node=0;
				for (;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f->arg_type[0]==argtype)
					{
						if (nterms-- == 0)
							break;
					}
				}

				// Copy subtree to temporary branch copy
				copyTree(copiedbranch, TreeNodeIter(&prgm->branch[br], node), -1);

				// Replace subtree with call to ADFarg
				TreeNode t=newarg->createInstance(); 

				copyTree(TreeNodeIter(&prgm->branch[br], node), t);	// Copy ADFarg function in
			}

			// Now, find all calls to this function, and add argument with copied branch
			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f==modfunc)
					{
						prgm->branch[br][node].arity++;
						TreeNodeIter pnode=TreeNodeIter(&prgm->branch[br], node);
						TreeNodeIter replacedargnode = pnode.end();
						TreeNodeIter sub = replacedargnode;
						copyTree(pnode.end(), copiedbranch, -(pnode->nodes));
						TreeNodeIter subend=sub.end();
						// Step through copied tree and replace ADF arguments with argument instances
						while (sub.node < subend.node)
						{
							ADFarg* myarg=dynamic_cast<ADFarg*>(sub.tree->at(sub.node).f);
							if (myarg!=NULL)
							{
								// get argument tree
								TreeNodeIter argnode=pnode.begin();
								for (int argi=0;argi<myarg->argNo;argi++)
									++argnode;
								copyTree(sub, argnode);
								subend=replacedargnode.end();
								++sub;
							}else
								sub.node++;
						}
					}
				}
				// Only need to look at branch 0 and then branch selfunc + 2 and beyond
				if (system->recursiveDepth<=0 && br==0)
					br=funcbr;	// Skip to end of deleted function branch
			}


#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}
	}
	else if (alterop==4)
	{
		// Argument duplication
		//
		// Duplicate function entry
		//cerr << "Duplicating argument" << endl;
		ADF* adf_func = (ADF*)(p->functions[dsti][selfunc]);
		int argtype = adf_func->arg_type[selarg+1];
		adf_func->arg_type.insert(adf_func->arg_type.begin()+selarg+2, argtype);
		ADFarg* oldarg = (ADFarg*)(adf_func->args[selarg]);
		ADFarg* newarg = new ADFarg(selarg+1, argtype);
		for (int i=selarg+1;i<adf_func->args.size();++i)
			((ADFarg*)(adf_func->args[i]))->argNo++;
		int funcbr = system->adfs + selfunc + 1;
		adf_func->args.insert(adf_func->args.begin()+selarg+1, newarg);
	
		for (int pi=0;pi<dst->size();++pi)
		{
			Program* prgm = (Program*)(dst->at(pi));

			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f==adf_func)
					{
		//				cerr << "Found invocation" << endl;
						// Select selarg'th argument
						TreeNodeIter pnode=TreeNodeIter(&prgm->branch[br], node);
						TreeNodeIter cnode=pnode.begin();
						for (int curarg=0;curarg<selarg;curarg++)
						{
							++cnode;
						}

						// Then copy that node to the next node
						TreeNodeIter nnode=cnode;
						++nnode;
						copyTree(nnode, cnode, pnode.node - nnode.node);
						pnode->arity++;
					}else if (prgm->branch[br][node].f==oldarg){
						if (rand()%2==0)
							prgm->branch[br][node].f=newarg;
					}
				}
				// Only need to look at branch 0 and then branch selfunc + 2 and beyond
				if (br==0)
					br=funcbr-1;	// Skip to beginning of deleted function branch
			}


#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}

	}
	else if (alterop==5)
	{
		// Argument deletion
		ADF* adf_func = (ADF*)(p->functions[dsti][selfunc]);
		int argtype = adf_func->arg_type[selarg+1];
		adf_func->arg_type.erase(adf_func->arg_type.begin()+selarg+1);
		ADFarg* oldarg = (ADFarg*)(adf_func->args[selarg]);
		for (int i=selarg+1;i<adf_func->args.size();++i)
			((ADFarg*)(adf_func->args[i]))->argNo--;
		int funcbr = system->adfs + selfunc + 1;
		adf_func->args.erase(adf_func->args.begin()+selarg);
	
		vector<TreeNode> arginstance;
		for (int pi=0;pi<dst->size();++pi)
		{
			arginstance.clear();
			Program* prgm = (Program*)(dst->at(pi));
			// cerr << "Deleting arg"<<selarg<<" ("<< oldarg<<" = "<<oldarg->name<<") from adf "<<(selfunc+system->adfs)<<" in branch "<<funcbr<<endl << endl << *prgm << endl << endl;

			for (int br=0;br<prgm->branch.size();++br)
			{
				for (int node=0;node<prgm->branch[br].size();++node)
				{
					if (prgm->branch[br][node].f==adf_func)
					{
//						cerr << "Found invocation at branch " << br <<", node "<<node <<endl;
						// Select selarg'th argument
						TreeNodeIter pnode=TreeNodeIter(&prgm->branch[br], node);
#ifdef DEBUG
		// Verify old function arity
		if (pnode->arity != adf_func->arg_type.size())
			throw runtime_error("Incorrect old function arity");
#endif
						TreeNodeIter cnode=pnode.begin();
						for (int curarg=0;curarg<selarg;curarg++)
						{
							++cnode;
						}

						/*if (br==0 && arginstance.size()==0)
						 * Can't copy invocation because it may contain invocations of itself
						{
							// Copy one valid instantiation of the argument
							copyTree(TreeNodeIter(&arginstance, 0), cnode, -1);
						}*/

						// Then erase that entire node
						eraseTree(cnode);
#ifdef DEBUG
		// Verify new function arity
		if (pnode->arity != adf_func->arg_type.size()-1)
			throw runtime_error("Incorrect new function arity");
#endif
					}
				}
				// Only need to look at branch 0 and then branch selfunc + 2 and beyond
				if (br==0)
					br=funcbr;	// Skip to beginning of deleted function branch
			}

			if (arginstance.size()==0)
			{
				//p->prepareADFs(system, dsti, funcbr);
				getGrower()->growTree(system, TreeNodeIter(&arginstance, 0), argtype);
				//p->clearADFs(system, dsti);
			}

			for (int node=0;node<prgm->branch[funcbr].size();++node)
			{
				//cerr << prgm->branch[funcbr][node].f->name<<" "<<prgm->branch[funcbr][node].f << " ";
				if (prgm->branch[funcbr][node].f==oldarg){

					// Occurrences of the argument must be replaced with something
					if (arginstance.size()==0)
					{
						throw runtime_error("No instantiation found to replace argument value with, and random generation for some reason wasn't used");
					}
					copyTree(TreeNodeIter(&prgm->branch[funcbr], node), TreeNodeIter(&arginstance, 0));
				}
			}

			//string tmp;
			//getline(cin, tmp);
#ifdef DEBUG
			if (!prgm->checkConsistency())
				throw runtime_error("Resulting program from architecture altering operation is not consistent");
#endif
		}
		delete oldarg;
	}
	else
		throw runtime_error("Architecture altering operation not implemented yet");
}

BreedPopOperator* AlterArchitecture::clone()
{
	return new AlterArchitecture();
}

