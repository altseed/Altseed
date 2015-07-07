#include <Altseed.h>
#include <gtest/gtest.h>
#include "../../EngineTest.h"

static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float3			g_values		: register( c0 );

struct PS_Input
{
	float4 SV_Position	: SV_POSITION;
	float4 Position		: POSITION;
	float2 UV			: UV;
	float4 Color		: COLOR;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float4 outputedColor = g_texture.Sample(g_sampler, Input.UV);
	if(outputedColor.a == 0.0f) discard;

	float2 localPos = float2( (Input.Position.x + 1.0) / 2.0 * g_values.x, (Input.Position.y + 1.0) / 2.0 * g_values.y );
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

out vec4 inPosition;
out vec2 inUV;
out vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 outputedColor = texture(g_texture, inUV.xy);

	vec2 localPos = vec2( (inPosition.x + 1.0) / 2.0 * g_values.x, (inPosition.y + 1.0) / 2.0 * g_values.y );
	vec2 centerPos = vec2( g_values.x / 2.0, g_values.y / 2.0 );

	if( (localPos.x - centerPos.x) * (localPos.x - centerPos.x) + (localPos.y - centerPos.y) * (localPos.y - centerPos.y) < g_values.z * g_values.z )
	{
		outputedColor.r = 1.0 - outputedColor.r;
		outputedColor.g = 1.0 - outputedColor.g;
		outputedColor.b = 1.0 - outputedColor.b;
	}

	outOutput = outputedColor;
}

)";

class CustomPostEffect : public asd::PostEffect
{
public:
	std::shared_ptr<asd::Shader2D>		m_shader;
	std::shared_ptr<asd::Material2D>	m_material2d;

public:
	CustomPostEffect(asd::Graphics* g)
	{
		if (g->GetGraphicsDeviceType() == asd::GraphicsDeviceType::DirectX11)
		{
			m_shader = g->CreateShader2D(
				asd::ToAString(shader2d_dx_ps).c_str()
				);
		}
		else if (g->GetGraphicsDeviceType() == asd::GraphicsDeviceType::OpenGL)
		{
			m_shader = g->CreateShader2D(
				asd::ToAString(shader2d_gl_ps).c_str()
				);
		}
		else
		{
			assert(0);
		}

		m_material2d = g->CreateMaterial2D(m_shader);
	}

	void OnDraw(std::shared_ptr<asd::RenderTexture2D> dst, std::shared_ptr<asd::RenderTexture2D> src)
	{
		m_material2d->SetTexture2D(asd::ToAString("g_texture").c_str(), src);
		m_material2d->SetVector3DF(asd::ToAString("g_values").c_str(), asd::Vector3DF(640,480,200));
		DrawOnTexture2DWithMaterial(dst, m_material2d);
	}
};




class Graphics_CustomPostEffect : public EngineTest
{
public:
	float intensity;
	Graphics_CustomPostEffect(bool isOpenGLMode) :
		EngineTest(asd::ToAString("CustomPostEffect"), isOpenGLMode, 10)
	{}
protected:
	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto object = std::make_shared<asd::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		asd::Engine::ChangeScene(scene);

		auto g = asd::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(asd::Vector2DF(2, 2));

		auto pe = std::make_shared<CustomPostEffect>(g);
		layer->AddPostEffect(pe);
	}
};

ENGINE_TEST(Graphics, CustomPostEffect)

