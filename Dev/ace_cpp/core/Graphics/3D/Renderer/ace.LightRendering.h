
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class LightRendering
	{
	private:
		std::shared_ptr<ace::NativeShader_Imp>	shadowBlurXShader;
		std::shared_ptr<ace::NativeShader_Imp>	shadowBlurYShader;

		std::shared_ptr<ace::NativeShader_Imp>	directionalWithAmbientLightShader;
		std::shared_ptr<ace::NativeShader_Imp>	directionalLightShader;
		std::shared_ptr<ace::NativeShader_Imp>	ambientLightShader;

	public:
		LightRendering(Graphics* graphics);
		virtual ~LightRendering();

		void Draw(RenderedDirectionalLightObject3DProxy* lightP);
	};
}
