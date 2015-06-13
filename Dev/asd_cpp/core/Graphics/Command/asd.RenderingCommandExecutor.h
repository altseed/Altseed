
#pragma once

#include <asd.common.Base.h>
#include "../../asd.Core.Base.h"
#include "../../asd.Core.Base_Imp.h"

namespace asd
{
	class RenderingCommandExecutor
	{
	private:

	public:
		void Execute(Graphics* graphics, 
			Effekseer::Manager* effectManager,
			EffekseerRenderer::Renderer* effectRenderer,
			SpriteRenderer3D* spriteRenerer3D,
			std::vector<RenderingCommand*>& commands);
	};
}