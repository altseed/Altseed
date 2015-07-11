#include <Altseed.h>

// レイヤーにセピアカラーのポストエフェクトを適用する。
void PostEffect_Sepia()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("PostEffect_Sepia").c_str(), 640, 480, asd::EngineOption());
	
	// 画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// シーンクラスのインスタンスを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// レイヤークラスのインスタンスを生成する。
	auto layer = std::make_shared<asd::Layer2D>();

	// 画像描画オブジェクトのインスタンスを生成する。
	auto obj = std::make_shared<asd::TextureObject2D>();

	// オブジェクトの位置とテクスチャを設定する。
	obj->SetPosition(asd::Vector2DF(50, 50));
	obj->SetTexture(texture);

	// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	asd::Engine::ChangeScene(scene);
	scene->AddLayer(layer);
	layer->AddObject(obj);

	// レイヤーにセピアカラーのポストエフェクトを適用する。
	layer->AddPostEffect(std::make_shared<asd::PostEffectSepia>());
	
	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}
	
	// Altseedを終了する。
	asd::Engine::Terminate();
}