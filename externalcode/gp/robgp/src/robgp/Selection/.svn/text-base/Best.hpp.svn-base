#pragma once

#include "../SelectionMethod.hpp"

namespace robgp
{
namespace Selection
{
	class Best : public SelectionMethod
	{
	public:
		Best();
		~Best();

		virtual Selectable* select(SelectableList* choices, int position, Selectable* prev=NULL);

		virtual SelectionMethod* clone();
		
	};
}};

