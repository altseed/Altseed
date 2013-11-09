#include "ace.PostEffectSmoothing.h"

#include <vector>
namespace ace{


	PostEffectSmoothing::PostEffectSmoothing(Graphics *g)
	{

		static const char* shader2d_dx_ps_x = R"(

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

		static const char* shader2d_gl_ps_x = R"(

uniform sampler2D g_texture;

void main()
{
	vec4 output = texture2D(g_texture, inUV.xy);
	output += texture2D(g_texture, inUV.xy + vec2(1, 0));
	output += texture2D(g_texture, inUV.xy + vec2(-1, 0));
	output += texture2D(g_texture, inUV.xy + vec2(2, 0));
	output += texture2D(g_texture, inUV.xy + vec2(-2, 0));
	gl_FragColor = output / 5.0; 
}

)";


		static const char* shader2d_dx_ps_y = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 output = g_texture.Sample(g_sampler, Input.UV);
	if(output.a == 0.0f) discard;
	output += g_texture.Sample(g_sampler, Input.UV + float2(0, 1));
	output += g_texture.Sample(g_sampler, Input.UV + float2(0, -1));
	output += g_texture.Sample(g_sampler, Input.UV + float2(0, 2));
	output += g_texture.Sample(g_sampler, Input.UV + float2(0, -2));
	return output / 5.0;
}

)";

		static const char* shader2d_gl_ps_y = R"(

uniform sampler2D g_texture;

void main()
{
	vec4 output = texture2D(g_texture, inUV.xy);
	output += texture2D(g_texture, inUV.xy + vec2(0, 1));
	output += texture2D(g_texture, inUV.xy + vec2(0, -1));
	output += texture2D(g_texture, inUV.xy + vec2(0, 2));
	output += texture2D(g_texture, inUV.xy + vec2(0, -2));
	gl_FragColor = output / 5.0; 
}

)";


		std::vector<ace::ShaderVariableProperty> propsX, propsY;
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		propsX.push_back(prop_tex);
		propsY.push_back(prop_tex);

		if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_DX11)
		{
			m_shaderX = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps_x).c_str(),
				propsX
				);
		}
		else if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_GL)
		{
			// std::vector<ace::ShaderVariableProperty> prop;
			m_shaderX = g->CreateShader2D(
				ace::ToAString(shader2d_gl_ps_x).c_str(),
				propsX
				);
		}
		else
		{
			assert(0);
		}

		m_material2dX = g->CreateMaterial2D(m_shaderX);



		if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_DX11)
		{
			m_shaderY = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps_y).c_str(),
				propsY
				);
		}
		else if (g->GetGraphicsType() == ace::GRAPHICS_TYPE_GL)
		{
			// std::vector<ace::ShaderVariableProperty> prop;
			m_shaderY = g->CreateShader2D(
				ace::ToAString(shader2d_gl_ps_y).c_str(),
				propsY
				);
		}
		else
		{
			assert(0);
		}

		m_material2dY = g->CreateMaterial2D(m_shaderY);

		m_graphics = g;
	}

	void PostEffectSmoothing::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		
		m_material2dX->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		auto size = src->GetSize();
		auto type = src->GetType();
		if (size.X <= 0 || size.Y <= 0 || m_graphics == nullptr){ return; } // return if the source and/or the graphics are invalid
		auto tmp = m_graphics->CreateRenderTexture(size.X, size.Y, ace::eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		if (tmp->GetSize().X != size.X || tmp->GetSize().Y != size.Y || tmp->GetType() != type){ return; }
		DrawOnTexture2DWithMaterial(tmp, m_material2dX);

		m_material2dY->SetTexture2D(ace::ToAString("g_texture").c_str(), tmp);
		DrawOnTexture2DWithMaterial(dst, m_material2dY);
	}

}