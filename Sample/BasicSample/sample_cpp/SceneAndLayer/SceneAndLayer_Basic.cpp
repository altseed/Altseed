#include <Altseed.h>

// シーンとレイヤーを手動で設定するサンプル。
void SceneAndLayer_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("SceneAndLayer_Basic").c_str(), 640, 480, asd::EngineOption());

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

	// 描画するシーンを指定する。
	asd::Engine::ChangeScene(scene);

	// 描画するレイヤーをシーンに追加する。
	scene->AddLayer(layer);

	// 描画するオブジェクトをレイヤーに追加する。
	layer->AddObject(obj);
	
	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}
	
	// Altseedを終了する。
	asd::Engine::Terminate();
}