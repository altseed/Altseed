
#include "ace.CorePostEffect_Imp.h"
#include "../../Graphics/Common/2D/ace.PostEffectRenderer.h"

namespace ace
{
	CorePostEffect_Imp::CorePostEffect_Imp(Graphics* graphics)
		: m_renderer(nullptr)
	{
		m_renderer = PostEffectRenderer::Create(graphics);
	}

	CorePostEffect_Imp::~CorePostEffect_Imp()
	{
		SafeRelease(m_renderer);
	}

	void CorePostEffect_Imp::DrawOnTexture2DWithMaterial(RenderTexture2D* target, Material2D* material)
	{
		auto target_ = (RenderTexture_Imp*) target;
		auto material_ = (Material2D_Imp*) material;

		m_renderer->DrawOnTexture2DWithMaterial_Imp(
			target_,
			material_);
	}
}