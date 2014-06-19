
#include "ace.PostEffectLightBloom.h"

namespace ace{


	PostEffectLightBloom::PostEffectLightBloom(Graphics *g)
	{
		std::string baseShader = m_corePostEffect->GetLightBloomShader(g->GetGraphicsType());

		std::string shader = std::string("#define COPY 1\n") + baseShader;
		std::string shaderX = std::string("#define BLUR_X 1\n") + baseShader;
		std::string shaderY = std::string("#define BLUR_Y 1\n") + baseShader;

		m_shader = g->CreateShader2D(ace::ToAString(shader.c_str()).c_str());
		m_material = g->CreateMaterial2D(m_shader);

		m_shaderX = g->CreateShader2D(ace::ToAString(shaderX.c_str()).c_str());
		m_material2dX = g->CreateMaterial2D(m_shaderX);

		m_shaderY = g->CreateShader2D(ace::ToAString(shaderY.c_str()).c_str());
		m_material2dY = g->CreateMaterial2D(m_shaderY);

		m_graphics = g;
	}

	void PostEffectLightBloom::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		assert(m_graphics != nullptr);
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

		if (m_copiedTexture == nullptr ||
			(m_copiedTexture->GetSize() != size || m_copiedTexture->GetFormat() != format))
		{
			if (format == eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT)
			{
				m_copiedTexture = m_graphics->CreateRenderTexture(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT);
			}
			else
			{
				m_copiedTexture = m_graphics->CreateRenderTexture(size.X, size.Y, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			}
		}

		m_material->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), src);
		DrawOnTexture2DWithMaterial(m_copiedTexture, m_material);
		

		const eTextureFilterType origSrcFiter = src->GetFilter();
		src->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);

		m_material2dX->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), src);
		m_material2dX->SetVector4DF(ace::ToAString("g_weight").c_str(), weights);

		m_tempTexture->SetFilter(eTextureFilterType::TEXTURE_FILTER_LINEAR);

		DrawOnTexture2DWithMaterial(m_tempTexture, m_material2dX);

		m_material2dY->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), m_tempTexture);
		m_material2dY->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), m_copiedTexture);
		m_material2dY->SetVector4DF(ace::ToAString("g_weight").c_str(), weights);

		DrawOnTexture2DWithMaterial(dst, m_material2dY);
		m_tempTexture->SetFilter(origSrcFiter);
	}

}