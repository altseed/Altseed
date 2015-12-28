
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class OceanRendering
	{
	private:

		struct Vertex
		{
			Vector3DF Position;
			Vector3DF Normal;
			Vector3DF Binormal;
			Vector2DF UV;
			Vector2DF UVSub;

		};

		std::shared_ptr<asd::NativeShader_Imp>	shader;
		std::shared_ptr<asd::VertexBuffer_Imp>	vertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	indexBuffer;

	public:
		OceanRendering(Graphics* graphics);
		virtual ~OceanRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, Matrix44 cameraMat, Matrix44 projMat, Texture2D* maskTexture, Texture2D* reflectionTexture, Texture2D* refractionTexture, RenderTexture2D_Imp* gb2, Texture2D* normalMap);

		void GenerateOcean(Vector2DF starting, Vector2DF ending, float height, float gridSize);

		Color OceanColor;
		float Density = 1.0f;
	};
}