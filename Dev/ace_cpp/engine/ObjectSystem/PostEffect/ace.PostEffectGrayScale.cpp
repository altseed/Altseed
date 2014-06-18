#include "ace.PostEffectGrayScale.h"

#include <vector>
namespace ace{

	PostEffectGrayScale::PostEffectGrayScale(Graphics *g)
	{
		std::vector<ace::ShaderVariableProperty> props;
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		props.push_back(prop_tex);

		m_shader = g->CreateShader2D(
			ToAString(m_corePostEffect->GetGrayScaleShader(g->GetGraphicsType())).c_str(),
			props
			);

		m_material2d = g->CreateMaterial2D(m_shader);
	}

	void PostEffectGrayScale::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		m_material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);

		DrawOnTexture2DWithMaterial(dst, m_material2d);
	}

}
