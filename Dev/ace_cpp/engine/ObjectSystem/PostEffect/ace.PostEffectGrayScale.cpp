#include "ace.PostEffectGrayScale.h"
#include "../../ace.Engine.h"

namespace ace{

	PostEffectGrayScale::PostEffectGrayScale()
	{
		auto g = Engine::GetGraphics();
		auto shader = g->CreateShader2D(ToAString(m_corePostEffect->GetGrayScaleShader(g->GetGraphicsDeviceType())).c_str());
		material2d = g->CreateMaterial2D(shader);
	}

	void PostEffectGrayScale::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		DrawOnTexture2DWithMaterial(dst, material2d);
	}

}
