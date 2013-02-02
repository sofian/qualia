#pragma once
#include "../TreeSelectionMethod.hpp"

namespace robgp
{
namespace TreeSelection
{
	class SizeFair : public TreeSelectionMethod
	{
	public:
		SizeFair();
		~SizeFair();

		virtual int selectTree(int branch, Program* p, Program* prev, int prevsel, int type=-1);
		virtual TreeSelectionMethod* clone();

	};
}

}

