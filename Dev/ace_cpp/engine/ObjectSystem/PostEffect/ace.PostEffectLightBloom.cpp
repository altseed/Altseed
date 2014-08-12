
#include "ace.PostEffectLightBloom.h"
#include "../../ace.Engine.h"

namespace ace{


	PostEffectLightBloom::PostEffectLightBloom()
	{
		auto g = Engine::GetGraphics();
		std::string baseCode = m_corePostEffect->GetLightBloomShader(g->GetGraphicsDeviceType());

		std::string code = std::string("#define COPY 1\n") + baseCode;
		std::string codeX = std::string("#define BLUR_X 1\n") + baseCode;
		std::string codeY = std::string("#define BLUR_Y 1\n") + baseCode;

		auto shader = g->CreateShader2D(ace::ToAString(code.c_str()).c_str());
		material = g->CreateMaterial2D(shader);

		auto shaderX = g->CreateShader2D(ace::ToAString(codeX.c_str()).c_str());
		material2dX = g->CreateMaterial2D(shaderX);

		auto shaderY = g->CreateShader2D(ace::ToAString(codeY.c_str()).c_str());
		material2dY = g->CreateMaterial2D(shaderY);
	}

	void PostEffectLightBloom::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		assert(src != nullptr);
		assert(dst != nullptr);

		Vector4DF weights;
		float ws[4];
		float total = 0.0f;
		float const dispersion = intensity * intensity;
		for (int32_t i = 0; i < 4; i++)
		{
			float pos = 1.0f + 2.0f * i;
			ws[i] = expf(-0.5f * pos * pos / dispersion);
			total += ws[i] * 2.0f;
		}
		weights.X = ws[0] / total;
		weights.Y = ws[1] / total;
		weights.Z = ws[2] / total;
		weights.W = ws[3] / total;

		auto size = src->GetSize();
		auto format = src->GetFormat();

		if (tempTexture == nullptr ||
			(tempTexture->GetSize() != size || tempTexture->GetFormat() != format))
		{
			if (format ==TextureFormat::R32G32B32A32_FLOAT)
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R32G32B32A32_FLOAT);
			}
			else
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		if (copiedTexture == nullptr ||
			(copiedTexture->GetSize() != size || copiedTexture->GetFormat() != format))
		{
			if (format == TextureFormat::R32G32B32A32_FLOAT)
			{
				copiedTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R32G32B32A32_FLOAT);
			}
			else
			{
				copiedTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		material->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), src);
		DrawOnTexture2DWithMaterial(copiedTexture, material);
		
		material2dX->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), src);
		material2dX->SetVector4DF(ace::ToAString("g_weight").c_str(), weights);
		material2dX->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dX->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dX->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(tempTexture, material2dX);

		material2dY->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), tempTexture);
		material2dY->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), copiedTexture);
		material2dY->SetVector4DF(ace::ToAString("g_weight").c_str(), weights);
		material2dY->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dY->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dY->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		material2dY->SetTextureFilterType(ace::ToAString("g_originalTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(dst, material2dY);
	}

}