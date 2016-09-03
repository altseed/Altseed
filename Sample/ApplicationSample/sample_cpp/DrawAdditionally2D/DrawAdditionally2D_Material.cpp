#include <Altseed.h>

// 専用のシェーダーを使用してスプライトを追加で描画するサンプル。

/**
@brief	DirectX向けシェーダー(HLSLで記述)
*/
static const char* shader2d_dx_ps = R"(
Texture2D g_texture : register( t0 );
SamplerState g_sampler : register( s0 );

struct PS_Input
{
	float4 SV_Position : SV_POSITION;
	float4 Position : POSITION;
	float2 UV : UV;
	float4 Color : COLOR;
};

float4 main( const PS_Input Input ) : SV_Target
{
	float4 color = g_texture.Sample(g_sampler, Input.UV);
	return float4( 1.0 - color.x, 1.0 - color.y, 1.0 - color.z, color.w);
}

)";

/**
@brief	OpenGL向けシェーダー(GLSLで記述)
*/
static const char* shader2d_gl_ps = R"(
uniform sampler2D	g_texture;

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

void main()
{
	vec4 color = texture(g_texture, inUV.xy);
	outOutput = vec4( 1.0 - color.x, 1.0 - color.y, 1.0 - color.z, color.w);
}

)";

/**
@brief	階調を反転して画像を描画するオブジェクト
*/
class InvertedDrawnObject2D : public asd::TextureObject2D
{
public:
	std::shared_ptr<asd::Shader2D>		shader;
	std::shared_ptr<asd::Material2D>	material2d;
	std::shared_ptr<asd::Texture2D>		texture;
public:
	InvertedDrawnObject2D()
	{
		// シェーダーをHLSL/GLSLから生成する。
		if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::DirectX11)
		{
			shader = asd::Engine::GetGraphics()->CreateShader2D(
				asd::ToAString(shader2d_dx_ps).c_str()
				);
		}
		else if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::OpenGL)
		{
			shader = asd::Engine::GetGraphics()->CreateShader2D(
				asd::ToAString(shader2d_gl_ps).c_str()
				);
		}

		material2d = asd::Engine::GetGraphics()->CreateMaterial2D(shader);

		// 画像を読み込む。
		texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

		// 画像を設定する。
		material2d->SetTexture2D(asd::ToAString("g_texture").c_str(), texture);
		material2d->SetTextureFilterType(asd::ToAString("g_texture").c_str(), asd::TextureFilterType::Linear);
		material2d->SetTextureWrapType(asd::ToAString("g_texture").c_str(), asd::TextureWrapType::Repeat);
	}

protected:
	void OnDrawAdditionally() override
	{
		// 画像を描画する。
		DrawSpriteWithMaterialAdditionally(
			asd::Vector2DF(100, 100),
			asd::Vector2DF(400, 100),
			asd::Vector2DF(400, 400),
			asd::Vector2DF(100, 400),
			asd::Color(255, 255, 255, 255),
			asd::Color(255, 255, 255, 255),
			asd::Color(255, 255, 255, 255),
			asd::Color(255, 255, 255, 255),
			asd::Vector2DF(0.0f, 0.0f),
			asd::Vector2DF(1.0f, 0.0f),
			asd::Vector2DF(1.0f, 1.0f),
			asd::Vector2DF(0.0f, 1.0f),
			material2d,
			asd::AlphaBlendMode::Blend,
			0);
	}
};

void DrawAdditionally2D_Material()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("DrawAdditionally2D_Material").c_str(), 640, 480, asd::EngineOption());

	// シーン、レイヤー、画像を表示するオブジェクトを生成する。
	auto scene = std::make_shared<asd::Scene>();
	auto layer = std::make_shared<asd::Layer2D>();
	auto obj = std::make_shared<InvertedDrawnObject2D>();
	
	// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	asd::Engine::ChangeScene(scene);
	scene->AddLayer(layer);
	layer->AddObject(obj);

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();
	}

	asd::Engine::Terminate();
}