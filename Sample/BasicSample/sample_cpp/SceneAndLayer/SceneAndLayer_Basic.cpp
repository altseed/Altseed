#include <Altseed.h>

// シーンとレイヤーを手動で設定するサンプル。
void SceneAndLayer_Basic()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("SceneAndLayer_Basic").c_str(), 640, 480, asd::EngineOption());

	// テクスチャをロードする。
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

	// 描画するシーンを指定する。
	asd::Engine::ChangeScene(scene);

	// 描画するレイヤーをシーンに追加する。
	scene->AddLayer(layer);

	// 描画するオブジェクトをレイヤーに追加する。
	layer->AddObject(obj);

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();
	}

	asd::Engine::Terminate();
}