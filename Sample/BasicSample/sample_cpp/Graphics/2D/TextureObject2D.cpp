
#include <Altseed.h>

// TextureObject2Dのサンプル。TextureObject2Dを用いて画像を表示しています。
void TextureObject2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextureObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	// テクスチャを読み込む
	auto tex = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

	// TextureObject2Dのインスタンスを生成する
	auto obj = std::make_shared<asd::TextureObject2D>();

	// 描画に使うテクスチャを設定する
	obj->SetTexture(tex);

	// 描画位置を指定する
	obj->SetPosition(asd::Vector2DF(100, 100));

	// レイヤーへ追加する
	layer->AddObject(obj);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}