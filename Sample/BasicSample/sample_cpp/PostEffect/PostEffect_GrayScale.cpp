#include <Altseed.h>

// レイヤーにグレースケールのポストエフェクトを適用する。
void PostEffect_GrayScale()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("PostEffect_GrayScale").c_str(), 640, 480, asd::EngineOption());

	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// シーンのコンストラクタを呼び出す。
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーのコンストラクタを呼び出す。
	auto layer = std::make_shared<asd::Layer2D>();

	// オブジェクトのコンストラクタを呼び出す。
	auto obj = std::make_shared<asd::TextureObject2D>();

	// オブジェクトの位置とテクスチャを設定。
	obj->SetPosition(asd::Vector2DF(50, 50));
	obj->SetTexture(texture);

	// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	asd::Engine::ChangeScene(scene);
	scene->AddLayer(layer);
	layer->AddObject(obj);

	// レイヤーにグレースケールのポストエフェクトを適用。
	layer->AddPostEffect(std::make_shared<asd::PostEffectGrayScale>());

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();
	}

	asd::Engine::Terminate();
}