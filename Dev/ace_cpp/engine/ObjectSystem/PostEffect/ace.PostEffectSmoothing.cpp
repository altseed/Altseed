#include "ace.PostEffectSmoothing.h"

#include <vector>
namespace ace{


	PostEffectSmoothing::PostEffectSmoothing(Graphics *g)
	{

		static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 output = g_texture.Sample(g_sampler, Input.UV);
	if(output.a == 0.0f) discard;
	output += g_texture.Sample(g_sampler, Input.UV + float2(1, 0));
	output += g_texture.Sample(g_sampler, Input.UV + float2(-1, 0));
	output += g_texture.Sample(g_sampler, Input.UV + float2(2, 0));
	output += g_texture.Sample(g_sampler, Input.UV + float2(-2, 0));
	return output / 5.0;
}

)";

		static const char* shader2d_gl_ps = R"(

uniform sampler2D g_texture;

void main()
{
	float4 output = texture2D(g_texture, inUV.xy);
	output += texture2D(g_texture, inUV.xy + float2(1, 0));
	output += texture2D(g_texture, inUV.xy + float2(-1, 0));
	output += texture2D(g_texture, inUV.xy + float2(2, 0));
	output += texture2D(g_texture, inUV.xy + float2(2, 0));
	gl_FragColor = output / 5.0; 
}

)";


		std::vector<ace::ShaderVariableProperty> props;
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		props.push_back(prop_tex);

		if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_DX11)
		{
			m_shader = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps).c_str(),
				props
				);
		}
		else if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_GL)
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

	void PostEffectSmoothing::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		m_material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		ace::Vector2DI size = src->GetSize();
		auto type = src->GetType();
		if(size)
		DrawOnTexture2DWithMaterial(dst, m_material2d);
	}

}