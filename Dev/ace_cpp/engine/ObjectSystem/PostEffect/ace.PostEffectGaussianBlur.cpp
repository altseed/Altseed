
#include "ace.PostEffectGaussianBlur.h"

namespace ace{


	PostEffectGaussianBlur::PostEffectGaussianBlur(Graphics *g)
	{

		static const char* shader2d_dx_ps_x = R"(
Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float3          g_weight        : register( c0 );
float4 main( const PS_Input Input ) : SV_Target
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	float2 accum = float2(1.0 / width, 0.0);
	float2 half_ = float2(0.5 / width, 0.0);
	float2 adder = float2(2.0 / width, 0.0);
	
	float4 output_ = (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	                 g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	                 g_weight.x;
	if(output_.a == 0.0f) discard;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.y;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.z;

	output_.a = 1.0;

	return output_;
}
)";

		static const char* shader2d_gl_ps_x = R"(
uniform sampler2D g_texture;
uniform vec3      g_weight;
void main()
{
	vec2 accum = vec2(1.0 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 half_  = vec2(0.5 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 adder = vec2(2.0 / float(textureSize(g_texture, 0).x), 0.0);
	vec4 output_ = (texture2D(g_texture, inUV.xy + half_ + accum)  +
	               texture2D(g_texture, inUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.z;

	output_.a = 1.0;

	gl_FragColor = output_; 
}

)";


		static const char* shader2d_dx_ps_y = R"(
Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float3          g_weight        : register( c0 );
float4 main( const PS_Input Input ) : SV_Target
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	float2 accum = float2(0.0, 1.0 / height);
	float2 half_ = float2(0.0, 0.5 / height);
	float2 adder = float2(0.0, 2.0 / height);
	
	float4 output_ = (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	                 g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	                 g_weight.x;
	if(output_.a == 0.0f) discard;

	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.y;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.z;

	output_.a = 1.0;

	return output_;
}
)";

		static const char* shader2d_gl_ps_y = R"(
uniform sampler2D g_texture;
uniform vec3      g_weight;
void main()
{
	vec2 accum = vec2(0.0, 1.0 / float(textureSize(g_texture, 0).y));
	vec2 half_  = vec2(0.0, 0.5 / float(textureSize(g_texture, 0).y));
	vec2 adder = vec2(0.0, 2.0 / float(textureSize(g_texture, 0).y));
	
	vec4 output_ = (texture2D(g_texture, inUV.xy + half_ + accum)  +
	               texture2D(g_texture, inUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.z;

	output_.a = 1.0;

	gl_FragColor = output_; 
}

)";


		std::vector<ace::ShaderVariableProperty> propsX, propsY;
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		
		auto prop_weight = ace::ShaderVariableProperty();
		prop_weight.Name = ace::ToAString("g_weight").c_str();
		prop_weight.Offset = 0;
		prop_weight.Type = ace::SHADER_VARIABLE_TYPE_VECTOR3DF;

		propsX.push_back(prop_tex);
		propsX.push_back(prop_weight);
		propsY.push_back(prop_tex);
		propsY.push_back(prop_weight);

		if (g->GetGraphicsType() == ace::GraphicsType::DirectX11)
		{
			m_shaderX = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps_x).c_str(),
				propsX
				);
		}
		else if (g->GetGraphicsType() == ace::GraphicsType::OpenGL)
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

		

		if (g->GetGraphicsType() == ace::GraphicsType::DirectX11)
		{
			m_shaderY = g->CreateShader2D(
				ace::ToAString(shader2d_dx_ps_y).c_str(),
				propsY
				);
		}
		else if (g->GetGraphicsType() == ace::GraphicsType::OpenGL)
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

		m_material2dX->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
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
		
		m_material2dY->SetTexture2D(ace::ToAString("g_texture").c_str(), m_tempTexture);
		m_material2dY->SetVector3DF(ace::ToAString("g_weight").c_str(), weights);
		
		DrawOnTexture2DWithMaterial(dst, m_material2dY);
		m_tempTexture->SetFilter(origSrcFiter);
	}

}