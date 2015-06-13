
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class SSAO
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	ssaoShader;
		std::shared_ptr<asd::NativeShader_Imp>	ssaoBlurXShader;
		std::shared_ptr<asd::NativeShader_Imp>	ssaoBlurYShader;

	public:
		SSAO(Graphics* graphics);
		virtual ~SSAO();

		bool IsEnabled();

		void Draw(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cameraP, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib);
		float Radius = 0.1f;
		float Bias = 0.001f;
		float Intensity = 1.0f;
		float FarPlain = 800.0f;
	};
}
