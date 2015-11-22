
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class SkyCubeRendering
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	shader;
		std::shared_ptr<asd::VertexBuffer_Imp>	vertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	indexBuffer;

	public:
		SkyCubeRendering(Graphics* graphics, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib);
		virtual ~SkyCubeRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, CubemapTexture* texture);
	};
}