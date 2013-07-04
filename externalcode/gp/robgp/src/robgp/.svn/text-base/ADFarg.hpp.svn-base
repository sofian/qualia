#pragma once
#include "Function.hpp"
#include "Frame.hpp"

namespace robgp
{

/**
 * This class provides the specification for an argument in an ADF as a
 * usable GP function. The argument values themselves will be stored in
 * the frame at execution time for retrieval.
 */

class ADFarg : public Function
{
public:
    ADFarg(int argno, int type);
    ~ADFarg();

    /**
     * Evaluates the ADF argument.
     */
    virtual Datatype eval(TreeNodeIter curnode, Frame* frame);
    
    int argNo;
};

}

