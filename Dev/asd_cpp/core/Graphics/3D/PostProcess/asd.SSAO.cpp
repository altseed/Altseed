
#include "asd.SSAO.h"
#include "../../asd.Graphics_Imp.h"

#include "../../Resource/asd.ShaderCache.h"

#include "../Object/asd.RenderedCameraObject3D.h"

#include "../../Command/asd.RenderingCommandHelper.h"

#include "../../Shader/DX/3D/PostProcess/SSAO_PS.h"
#include "../../Shader/DX/3D/PostProcess/SSAO_Blur_PS.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Screen_VS.h"


namespace asd
{
	SSAO::SSAO(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));

		std::vector<asd::Macro> macro;
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{

		}
		else
		{
			ssaoShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.SSAO").c_str(),
				screen_vs_dx,
				ssao_ps_dx,
				vl,
				macro);

			const char* BLUR_X = "BLUR_X";
			const char* BLUR_Y = "BLUR_Y";
			const char* ONE = "1";

			macro.push_back(Macro(BLUR_X, ONE));
			ssaoBlurXShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.SSAO.BlurX").c_str(),
				screen_vs_dx,
				ssao_blur_ps_dx,
				vl,
				macro);

			macro.clear();
			macro.push_back(Macro(BLUR_Y, ONE));
			ssaoBlurYShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.SSAO.BlurY").c_str(),
				screen_vs_dx,
				ssao_blur_ps_dx,
				vl,
				macro);

		}
	}

	SSAO::~SSAO()
	{

	}


	bool SSAO::IsEnabled()
	{
		return ssaoShader.get() != nullptr;
	}


	void SSAO::Draw(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cameraP, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib)
	{
		using h = RenderingCommandHelper;

		helper->SetRenderTarget(cameraP->GetRenderTargetSSAO(), nullptr);
		helper->Clear(true, false, asd::Color(0, 0, 0, 255));

		auto size_ = Vector4DF(cameraP->WindowSize.X, cameraP->WindowSize.Y, 0.0f, 0.0f);
		auto fov = cameraP->FOV / 180.0f * 3.141592f;
		auto aspect = (float) cameraP->WindowSize.X / (float) cameraP->WindowSize.Y;

		// DirectX
		float yScale = 1 / tanf(fov / 2);
		float xScale = yScale / aspect;

		auto reconstructInfo1 = Vector3DF(cameraP->ZNear * cameraP->ZFar, cameraP->ZFar - cameraP->ZNear, -cameraP->ZFar);
		//auto reconstructInfo1 = Vector3DF(cP->ZFar - cP->ZNear, cP->ZNear, 0.0f);
		auto reconstructInfo2 = Vector4DF(1.0f / xScale, 1.0f / yScale, 0.0f, 0.0f);

		float projScale = cameraP->WindowSize.Y * yScale / 2.0f;

		RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;
		state.Culling = CullingType::Double;

		helper->Draw(2, vb.get(), ib.get(), ssaoShader.get(), state,
			h::GenValue("radius", Radius),
			h::GenValue("projScale", projScale),
			h::GenValue("bias", Bias),
			h::GenValue("intensity", Intensity),
			h::GenValue("farPlain", FarPlain),
			h::GenValue("reconstructInfo1", reconstructInfo1),
			h::GenValue("reconstructInfo2", reconstructInfo2),
			h::GenValue("g_texture", h::Texture2DPair(cameraP->GetRenderTargetDepth(), asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));

		{
			helper->SetRenderTarget(cameraP->GetRenderTargetSSAO_Temp(), nullptr);
			helper->Clear(true, false, asd::Color(0, 0, 0, 255));

			RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			state.Culling = CullingType::Double;

			helper->Draw(2, vb.get(), ib.get(), ssaoBlurXShader.get(), state,
				h::GenValue("g_texture", h::Texture2DPair(cameraP->GetRenderTargetSSAO(), asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));
		}

		{
			helper->SetRenderTarget(cameraP->GetRenderTargetSSAO(), nullptr);
			helper->Clear(true, false, asd::Color(0, 0, 0, 255));

			RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			state.Culling = CullingType::Double;

			helper->Draw(2, vb.get(), ib.get(), ssaoBlurYShader.get(), state,
				h::GenValue("g_texture", h::Texture2DPair(cameraP->GetRenderTargetSSAO_Temp(), asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));
		}
	}
}