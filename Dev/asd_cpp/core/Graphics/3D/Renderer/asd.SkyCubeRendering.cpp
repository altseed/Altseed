
#include "asd.SkyCubeRendering.h"

#include "../../asd.Graphics_Imp.h"
#include "../../Resource/asd.ShaderCache.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/DX/3D/SkyCube_PS.h"

#include "../../Command/asd.RenderingCommandHelper.h"

#include "../Object/asd.RenderedCameraObject3D.h"

namespace asd
{

	SkyCubeRendering::SkyCubeRendering(Graphics* graphics, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib)
	{
		auto g = (Graphics_Imp*) graphics;
		vertexBuffer = vb;
		indexBuffer = ib;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));

		std::vector<asd::Macro> macro;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			return;
		}
		else
		{
			shader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.SkyCube").c_str(),
				screen_vs_dx,
				skycube_ps_dx,
				vl,
				macro);
		}
	}

	SkyCubeRendering::~SkyCubeRendering()
	{

	}

	void SkyCubeRendering::Render(Matrix44 cameraMat, Matrix44 projMat, Vector3DF position, RenderingCommandHelper* helper, CubemapTexture* texture)
	{
		if (shader == nullptr) return;
		if (texture == nullptr) return;

		using h = RenderingCommandHelper;

		RenderState state;
		state.DepthTest = true;
		state.DepthWrite = false;
		state.AlphaBlendState = AlphaBlendMode::Blend;
		state.Culling = asd::CullingType::Double;

		auto mat = projMat * cameraMat;
		mat.SetInverted();

		helper->Draw(2, vertexBuffer.get(), indexBuffer.get(), shader.get(), state,
			h::GenValue("g_cameraPosition", position),
			h::GenValue("g_cameraProjInvMat", mat),
			h::GenValue("g_skyTexture", h::CubemapTexturePair(texture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));
	}
}