
#include "../../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>

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

using namespace std;
using namespace asd;

class Graphics_TextureObject2DWithMaterial : public EngineTest
{
public:
	Graphics_TextureObject2DWithMaterial(bool isOpenGLMode) : EngineTest(u"TextureObject2DWithMaterial", isOpenGLMode, 20) {}

private:
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
		else { assert(0); }

		auto material = asd::Engine::GetGraphics()->CreateMaterial2D(shader);

		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(u"Data/Texture/Sample1.png");

		auto textureObj1 = make_shared<TextureObject2D>();
		auto textureObj2 = make_shared<TextureObject2D>();

		layer->AddObject(textureObj1);
		layer->AddObject(textureObj2);
		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);

		textureObj1->SetTexture(texture);
		textureObj1->SetSrc(asd::RectF(256, 256, 256, 256));
		textureObj1->SetPosition(asd::Vector2DF(320, 240));
		textureObj1->SetMaterial(material);

		textureObj2->SetTexture(texture);
		textureObj2->SetSrc(asd::RectF(0, 0, 256, 256));
		textureObj2->SetPosition(asd::Vector2DF(400, 320));
		textureObj2->SetTurnLR(true);
		textureObj2->SetMaterial(material);

	}

	void OnUpdating() {}
};

ENGINE_TEST(Graphics, TextureObject2DWithMaterial)
