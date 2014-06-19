
#include "ace.PostEffectGaussianBlur.h"

namespace ace{


	PostEffectGaussianBlur::PostEffectGaussianBlur(Graphics *g)
	{
		std::string baseShader = m_corePostEffect->GetGaussianBlurShader(g->GetGraphicsType());

		std::string shaderX = std::string("#define BLUR_X 1\n") + baseShader;
		std::string shaderY = std::string("#define BLUR_Y 1\n") + baseShader;

		m_shaderX = g->CreateShader2D(ace::ToAString(shaderX.c_str()).c_str());
		m_material2dX = g->CreateMaterial2D(m_shaderX);

		m_shaderY = g->CreateShader2D(ace::ToAString(shaderY.c_str()).c_str());
		m_material2dY = g->CreateMaterial2D(m_shaderY);

		m_graphics = g;
	}

	void PostEffectGaussianBlur::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		assert(m_graphics != nullptr);
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

		const eTextureFilterType origSrcFiter = src->GetFilter();
		src->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);

		m_material2dX->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), src);
		m_material2dX->SetVector3DF(ace::ToAString("g_weight").c_str(), weights);

		auto size = src->GetSize();
		auto format = src->GetFormat();
		
		if (m_tempTexture == nullptr ||
			(m_tempTexture->GetSize() != size || m_tempTexture->GetFormat() != format))
		{
			if (format == eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT)
			{
				m_tempTexture = m_graphics->CreateRenderTexture(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT);
			}
			else
			{
				m_tempTexture = m_graphics->CreateRenderTexture(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			}
		}

		m_tempTexture->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);
		
		DrawOnTexture2DWithMaterial(m_tempTexture, m_material2dX);
		
		m_material2dY->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), m_tempTexture);
		m_material2dY->SetVector3DF(ace::ToAString("g_weight").c_str(), weights);
		
		DrawOnTexture2DWithMaterial(dst, m_material2dY);
		m_tempTexture->SetFilter(origSrcFiter);
	}

}