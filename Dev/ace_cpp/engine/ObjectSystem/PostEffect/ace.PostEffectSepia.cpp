#include "ace.PostEffectSepia.h"
#include "../../ace.Engine.h"

namespace ace{

	// 現在のアルゴリズム:グレースケール化→セピア色(107, 74, 43)をグレースケール化した場合の
	// 明度とRGB各値の比を取り、グレースケール化した各画素の成分ごとに乗ずる
	PostEffectSepia::PostEffectSepia()
	{
		auto g = Engine::GetGraphics();
		auto shader = g->CreateShader2D(ToAString(m_corePostEffect->GetSepiaShader(g->GetGraphicsDeviceType())).c_str());
		material2d = g->CreateMaterial2D(shader);
	}

	void PostEffectSepia::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		DrawOnTexture2DWithMaterial(dst, material2d);
	}

}
