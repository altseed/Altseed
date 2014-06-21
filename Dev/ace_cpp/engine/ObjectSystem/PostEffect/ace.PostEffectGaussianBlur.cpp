
#include "ace.PostEffectGaussianBlur.h"
#include "../../ace.Engine.h"

namespace ace{


	PostEffectGaussianBlur::PostEffectGaussianBlur()
	{
		auto g = Engine::GetGraphics();
		std::string baseShader = m_corePostEffect->GetGaussianBlurShader(g->GetGraphicsType());

		std::string codeX = std::string("#define BLUR_X 1\n") + baseShader;
		std::string codeY = std::string("#define BLUR_Y 1\n") + baseShader;

		auto shaderX = g->CreateShader2D(ace::ToAString(codeX.c_str()).c_str());
		material2dX = g->CreateMaterial2D(shaderX);

		auto shaderY = g->CreateShader2D(ace::ToAString(codeY.c_str()).c_str());
		material2dY = g->CreateMaterial2D(shaderY);
	}

	void PostEffectGaussianBlur::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		assert(src != nullptr);
		assert(dst != nullptr);

		Vector3DF weights;
		float ws[3];
		float total = 0.0f;
		float const dispersion = intensity * intensity;
		for (int32_t i = 0; i < 3; i++)
		{
			float pos = 1.0f + 2.0f * i;
			ws[i] = expf(-0.5f * pos * pos / dispersion);
			total += ws[i] * 2.0f;
		}
		weights.X = ws[0] / total;
		weights.Y = ws[1] / total;
		weights.Z = ws[2] / total;

		src->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);

		material2dX->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		material2dX->SetVector3DF(ace::ToAString("g_weight").c_str(), weights);

		auto size = src->GetSize();
		auto format = src->GetFormat();
		
		if (tempTexture == nullptr ||
			(tempTexture->GetSize() != size || tempTexture->GetFormat() != format))
		{
			if (format == eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT)
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT);
			}
			else
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			}
		}

		tempTexture->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);
		
		DrawOnTexture2DWithMaterial(tempTexture, material2dX);
		
		material2dY->SetTexture2D(ace::ToAString("g_texture").c_str(), tempTexture);
		material2dY->SetVector3DF(ace::ToAString("g_weight").c_str(), weights);
		
		DrawOnTexture2DWithMaterial(dst, material2dY);
	}

}