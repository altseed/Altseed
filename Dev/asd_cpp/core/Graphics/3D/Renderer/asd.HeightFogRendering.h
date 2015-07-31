
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class HeightFogRendering
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	heightFogShader;
		std::shared_ptr<asd::VertexBuffer_Imp>	vertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	indexBuffer;

	public:
		HeightFogRendering(Graphics* graphics, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib);
		virtual ~HeightFogRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3);
	
		Color FogColor = Color(255,255,255,255);
		float StartDistance = 10.0f;
		float Density = 1.0f;
		float Falloff = 1.0f;
	};
}