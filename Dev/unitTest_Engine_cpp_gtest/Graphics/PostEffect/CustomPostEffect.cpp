#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"
static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float3			g_values		: register( c0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 outputedColor = g_texture.Sample(g_sampler, Input.UV);
	if(outputedColor.a == 0.0f) discard;

	float2 localPos = float2( (Input.Pos.x + 1.0) / 2.0 * g_values.x, (Input.Pos.y + 1.0) / 2.0 * g_values.y );
	float2 centerPos = float2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		outputedColor.r = 1.0 - outputedColor.r;
		outputedColor.g = 1.0 - outputedColor.g;
		outputedColor.b = 1.0 - outputedColor.b;
	}

	return outputedColor;
}

)";

static const char* shader2d_gl_ps = R"(

uniform sampler2D	g_texture;

uniform vec3		g_values;

void main()
{
	vec4 outputedColor = texture2D(g_texture, inUV.xy);

	vec2 localPos = vec2( (inPos.x + 1.0) / 2.0 * g_values.x, (inPos.y + 1.0) / 2.0 * g_values.y );
	vec2 centerPos = vec2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		outputedColor.r = 1.0 - outputedColor.r;
		outputedColor.g = 1.0 - outputedColor.g;
		outputedColor.b = 1.0 - outputedColor.b;
	}

	gl_FragColor = outputedColor; 
}

)";

class CustomPostEffect : public ace::PostEffect
{
public:
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




class Graphics_CustomPostEffect : public EngineTest
{
public:
	float intensity;
	Graphics_CustomPostEffect(bool isOpenGLMode) :
		EngineTest(ace::ToAString("CustomPostEffect"), isOpenGLMode, 10)
	{}
protected:
	void OnStart() override
	{
		auto engine = ace::GetEngine();

		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		engine->ChangeScene(scene);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(2, 2));

		auto pe = std::make_shared<CustomPostEffect>(g);
		layer->AddPostEffect(pe);
	}



};


TEST(Graphics, CustomPostEffect_GL)
{
	Graphics_CustomPostEffect(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Graphics, CustomPostEffect_DX)
{
	Graphics_CustomPostEffect(false).Run();
	AssertMemoryDoesntLeak();
}
#endif

void Graphics_CustomPostEffect_(bool isGL)
{
	Graphics_CustomPostEffect(isGL).Run();
	AssertMemoryDoesntLeak();
}

