#include "../../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace asd;

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

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

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


class Graphics_TextObject2DWithMaterial : public EngineTest
{
public:
	Graphics_TextObject2DWithMaterial(bool isOpenGLMode) : EngineTest(u"TextObject2DWithMaterial ", isOpenGLMode, 10) {}

protected:
	void OnStart()
	{
		std::shared_ptr<asd::Shader2D> shader;

		if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::DirectX11)
		{
			shader = asd::Engine::GetGraphics()->CreateShader2D(asd::ToAString(shader2d_dx_ps).c_str());
		}
		else if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::OpenGL)
		{
			shader = asd::Engine::GetGraphics()->CreateShader2D(asd::ToAString(shader2d_gl_ps).c_str());
		}
		else
		{
			assert(0);
		}

		auto material = asd::Engine::GetGraphics()->CreateMaterial2D(shader);

		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto g = asd::Engine::GetGraphics();

		asd::Engine::GetFile()->AddRootDirectory(u"Data");

		layer->AddObject(object);
		auto font = g->CreateFont(u"Font/Nac0812.aff");

		object->SetText(u"DependencyPropertyとは");

		// object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(0, 0));
		object->SetFont(font);
		object->SetMaterial(material);

		auto size = font->CalcTextureSize(u"DependencyPropertyとは", WritingDirection::Horizontal);

		printf("%d %d\n", size.X, size.Y);
	}
};

ENGINE_TEST(Graphics, TextObject2DWithMaterial)
