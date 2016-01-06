
#include "asd.OceanRendering.h"

#include "../../asd.Graphics_Imp.h"
#include "../../Resource/asd.ShaderCache.h"

#include "../../Resource/asd.VertexBuffer_Imp.h"
#include "../../Resource/asd.IndexBuffer_Imp.h"

#include "../../Command/asd.RenderingCommandHelper.h"

#include "../Object/asd.RenderedCameraObject3D.h"

#include "../../Shader/DX/3D/Ocean_VS.h"
#include "../../Shader/DX/3D/Ocean_PS.h"

namespace asd
{
	OceanRendering::OceanRendering(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;

		vertexBuffer = g->CreateVertexBuffer_Imp(sizeof(Vertex), 2 * 3, true);
		indexBuffer = g->CreateIndexBuffer_Imp(2 * 3, false, false);

		indexBuffer->Lock();
		auto ib = indexBuffer->GetBuffer<uint16_t>(2 * 3);

		for (int32_t i = 0; i < 2; i++)
		{
			ib[i * 3 + 0] = 0 + i * 3;
			ib[i * 3 + 1] = 1 + i * 3;
			ib[i * 3 + 2] = 2 + i * 3;
		}

		indexBuffer->Unlock();

		vertexBuffer->Lock();
		auto buf = vertexBuffer->GetBuffer <Vertex>(6);

		float depth = 1.0f;
		buf[0].Position = Vector3DF(-1.0f, 1.0f, depth);
		buf[0].UV = Vector2DF(0, 0);
		buf[0].UVSub = Vector2DF(0, 0);
		buf[1].Position = Vector3DF(1.0f, 1.0f, depth);
		buf[1].UV = Vector2DF(1, 0);
		buf[1].UVSub = Vector2DF(1, 0);
		buf[2].Position = Vector3DF(1.0f, -1.0f, depth);
		buf[2].UV = Vector2DF(1, 1);
		buf[2].UVSub = Vector2DF(1, 1);
		buf[3].Position = Vector3DF(-1.0f, -1.0f, depth);
		buf[3].UV = Vector2DF(0, 1);
		buf[3].UVSub = Vector2DF(0, 1);

		buf[4] = buf[0];
		buf[5] = buf[2];

		vertexBuffer->Unlock();

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("Normal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("Binormal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("UVSub", asd::VertexLayoutFormat::R32G32_FLOAT));

		std::vector<asd::Macro> macro;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			return;
		}
		else
		{
			shader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.Ocean").c_str(),
				ocean_vs_dx,
				ocean_ps_dx,
				vl,
				macro);
		}
	}

	OceanRendering::~OceanRendering()
	{

	}

	void OceanRendering::Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, Matrix44 cameraMat, Matrix44 projMat, Texture2D* maskTexture, Texture2D* reflectionTexture, Texture2D* refractionTexture, RenderTexture2D_Imp* gb2, Texture2D* normalMap)
	{
		if (shader == nullptr) return;

		using h = RenderingCommandHelper;

		RenderState state;
		state.DepthTest = true;
		state.DepthWrite = true;
		state.AlphaBlendState = AlphaBlendMode::Blend;
		state.Culling = asd::CullingType::Double;

		auto mat = projMat * cameraMat;
		mat.SetInverted();

		Matrix44 matM;
		Matrix44 matC = cameraMat;
		Matrix44 matP = projMat;

		Vector3DF color;
		color.X = OceanColor.R / 255.0f;
		color.Y = OceanColor.G / 255.0f;
		color.Z = OceanColor.B / 255.0f;

		helper->Draw(2, vertexBuffer.get(), indexBuffer.get(), shader.get(), state,

			h::GenValue("reconstructInfo1", cameraP->ReconstructInfo1),
			h::GenValue("reconstructInfo2", cameraP->ReconstructInfo2),

			h::GenValue("g_gbuffer2Texture", h::Texture2DPair(gb2, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),

			h::GenValue("g_reflectionTexture", h::Texture2DPair(reflectionTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),
			h::GenValue("g_refractionTexture", h::Texture2DPair(refractionTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),
			h::GenValue("g_normalTexture", h::Texture2DPair(normalMap, asd::TextureFilterType::Linear, asd::TextureWrapType::Repeat)),
			h::GenValue("g_maskTexture", h::Texture2DPair(maskTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Repeat)),

			h::GenValue("matM", matM),
			h::GenValue("matC", matC),
			h::GenValue("matP", matP),
			
			h::GenValue("g_oceanColor", color),

			h::GenValue("g_density", Density)
			);
	}

	void OceanRendering::GenerateOcean(Vector2DF starting, Vector2DF ending, float height, float gridSize)
	{
		auto sx = Min(starting.X, ending.X);
		auto sy = Min(starting.Y, ending.Y);
		auto ex = Max(starting.X, ending.X);
		auto ey = Max(starting.Y, ending.Y);

		vertexBuffer->Lock();
		auto buf = vertexBuffer->GetBuffer <Vertex>(6);

		float depth = 1.0f;
		buf[0].Position = Vector3DF(sx, height, sy);
		buf[0].UV = Vector2DF(0, 0);
		buf[0].Normal = Vector3DF(0, 1, 0);
		buf[0].Binormal = Vector3DF(0, 0, 1);
		buf[0].UVSub = Vector2DF(0, 0);

		buf[1].Position = Vector3DF(ex, height, sy);
		buf[1].UV = Vector2DF((ex - sx) / gridSize, 0);
		buf[1].Normal = Vector3DF(0, 1, 0);
		buf[1].Binormal = Vector3DF(0, 0, 1);
		buf[1].UVSub = Vector2DF(1, 0);

		buf[2].Position = Vector3DF(ex, height, ey);
		buf[2].UV = Vector2DF((ex - sx) / gridSize, (ey - sy) / gridSize);
		buf[2].Normal = Vector3DF(0, 1, 0);
		buf[2].Binormal = Vector3DF(0, 0, 1);
		buf[2].UVSub = Vector2DF(1, 1);

		buf[3].Position = Vector3DF(sx, height, ey);
		buf[3].UV = Vector2DF(0, (ey - sy) / gridSize);
		buf[3].Normal = Vector3DF(0, 1, 0);
		buf[3].Binormal = Vector3DF(0, 0, 1);
		buf[3].UVSub = Vector2DF(0, 1);

		buf[4] = buf[0];
		buf[5] = buf[2];

		vertexBuffer->Unlock();
	}
}