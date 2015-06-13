
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class LightRendering
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	shadowBlurXShader;
		std::shared_ptr<asd::NativeShader_Imp>	shadowBlurYShader;

		std::shared_ptr<asd::NativeShader_Imp>	directionalWithAmbientLightShader;
		std::shared_ptr<asd::NativeShader_Imp>	directionalLightShader;
		std::shared_ptr<asd::NativeShader_Imp>	ambientLightShader;

	public:
		LightRendering(Graphics* graphics);
		virtual ~LightRendering();

		void Draw(RenderingCommandHelper* helper, std::set<RenderedObject3DProxy*>& objects, RenderedCameraObject3DProxy* cameraP, RenderedDirectionalLightObject3DProxy* lightP, Color skyColor, Color groundColor);
		void Draw(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cameraP, Color skyColor, Color groundColor);

	};
}
