
#include <ace.h>
#include <gtest/gtest.h>
#include <memory>

static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float3			g_values		: register( c0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 output = g_texture.Sample(g_sampler, Input.UV);
	if(output.a == 0.0f) discard;

	float2 localPos = float2( (Input.Pos.x + 1.0) / 2.0 * g_values.x, (Input.Pos.y + 1.0) / 2.0 * g_values.y );
	float2 centerPos = float2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		output.r = 1.0 - output.r;
		output.g = 1.0 - output.g;
		output.b = 1.0 - output.b;
	}

	return output;
}

)";

static const char* shader2d_gl_ps = R"(

uniform sampler2D	g_texture;

uniform vec3		g_values;

void main()
{
	vec4 output = texture2D(g_texture, inUV.xy);

	vec2 localPos = vec2( (Input.Pos.x + 1.0) / 2.0 * g_values.x, (Input.Pos.y + 1.0) / 2.0 * g_values.y );
	vec2 centerPos = vec2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		output.r = 1.0 - output.r;
		output.g = 1.0 - output.g;
		output.b = 1.0 - output.b;
	}

	gl_FragColor = output; 
}

)";

class CustomPostEffect : public ace::PostEffect
{
private:
	std::shared_ptr<ace::Shader2D>		m_shader;
	std::shared_ptr<ace::Material2D>	m_material2d;

public:
	CustomPostEffect(ace::Graphics* g)
	{
		// 外部から設定可能な変数の設定
		std::vector<ace::ShaderVariableProperty> props;

		// テクスチャ
		auto prop_tex = ace::ShaderVariableProperty();
		prop_tex.Name = ace::ToAString("g_texture").c_str();
		prop_tex.Offset = 0;
		prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
		props.push_back(prop_tex);

		// 変数
		auto prop_v = ace::ShaderVariableProperty();
		prop_v.Name = ace::ToAString("g_values").c_str();
		prop_v.Offset = 0;
		prop_v.Type = ace::SHADER_VARIABLE_TYPE_VECTOR3DF;
		props.push_back(prop_v);

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

	void OnDraw(std::shared_ptr<ace::RenderTexture2D> dst, std::shared_ptr<ace::RenderTexture2D> src)
	{
		m_material2d->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		m_material2d->SetVector3DF(ace::ToAString("g_values").c_str(), ace::Vector3DF(640,480,200));
		DrawOnTexture2DWithMaterial(dst, m_material2d);
	}
};

void Graphics_CustomPostEffect()
{
	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("CustomPostEffect").c_str(), 640, 480, false);

	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		engine->ChangeScene(scene);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);

		auto pe = std::make_shared<CustomPostEffect>(g);
		layer->AddPostEffect(pe);

		while (engine->DoEvents())
		{
			engine->Update();
		}
	}

	engine->Terminate();

	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_TRUE(ref == 0);
}

TEST(Graphics, CustomPostEffect)
{
	Graphics_CustomPostEffect();
}