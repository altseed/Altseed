#include "ace.PostEffectGrayScale.h"

#include <vector>
namespace ace{

	PostEffectGrayScale::PostEffectGrayScale(Graphics *g)
	{

		static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 outputedColor = g_texture.Sample(g_sampler, Input.UV);
	if(outputedColor.a == 0.0f) discard;
	float y = outputedColor.r * 0.298912f + outputedColor.g * 0.586611f + outputedColor.b * 0.114478f;
	outputedColor.rgb = y;
	return outputedColor;
}

)";

static const char* shader2d_gl_ps = R"(

uniform sampler2D g_texture;

void main()
{
	vec4 outputedColor = texture2D(g_texture, inUV.xy);
	float y = outputedColor.r * 0.298912 + outputedColor.g * 0.586611 + outputedColor.b * 0.114478;
	//outputedColor.xyz = y; (特定環境でコンパイル不可)
	outputedColor.rgb = vec3(y,y,y);
	gl_FragColor = outputedColor; 
}

)";


		std::vector<ace::ShaderVariableProperty> props;
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		props.push_back(prop_tex);

		if (g->GetGraphicsType() == ace::GraphicsType::DirectX11)
		{
			m_shader = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps).c_str(),
				props
				);
		}
		else if (g->GetGraphicsType() == ace::GraphicsType::OpenGL)
		{
			std::vector<ace::ShaderVariableProperty> prop;
			m_shader = g->CreateShader2D(
				ace::ToAString(shader2d_gl_ps).c_str(),
				props
				);
		}
		else
		{
			assert(0);
		}

		m_material2d = g->CreateMaterial2D(m_shader);
	}

	void PostEffectGrayScale::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		m_material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);

		DrawOnTexture2DWithMaterial(dst, m_material2d);
	}

}
