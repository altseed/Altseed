
#pragma once

#include <ace.common.Base.h>
#include "../../ace.Core.Base.h"
#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	class RenderingCommandExecutor
	{
	private:

	public:
		void Execute(Graphics* graphics, std::vector<RenderingCommand*>& commands);
	};
}