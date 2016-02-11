#include <Altseed.h>

// 階調を反転するポストエフェクトのサンプル。

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
	@brief	階調を反転させるポストエフェクト
*/
class InvertPostEffect : public asd::PostEffect
{
public:
	std::shared_ptr<asd::Shader2D>		shader;
	std::shared_ptr<asd::Material2D>	material2d;

public:
	InvertPostEffect()
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
	}

protected:
	void OnDraw(std::shared_ptr<asd::RenderTexture2D> dst, std::shared_ptr<asd::RenderTexture2D> src) override
	{
		// マテリアルを経由してシェーダー内のg_texture変数に画面の画像(src)を入力する。
		material2d->SetTexture2D(asd::ToAString("g_texture").c_str(), src);

		// 出力画像(dst)に指定したマテリアルで描画する。
		DrawOnTexture2DWithMaterial(dst, material2d);
	}
};

void CustomPostEffect_Invert()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("CustomPostEffect_Invert").c_str(), 640, 480, asd::EngineOption());

	// シーン、レイヤー、画像を表示するオブジェクトを生成する。
	auto scene = std::make_shared<asd::Scene>();
	auto layer = std::make_shared<asd::Layer2D>();
	auto obj = std::make_shared<asd::TextureObject2D>();
	obj->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str()));

	// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	asd::Engine::ChangeScene(scene);
	scene->AddLayer(layer);
	layer->AddObject(obj);

	// レイヤーにポストエフェクトを適用する。
	layer->AddPostEffect(std::make_shared<InvertPostEffect>());

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();
	}

	asd::Engine::Terminate();
}