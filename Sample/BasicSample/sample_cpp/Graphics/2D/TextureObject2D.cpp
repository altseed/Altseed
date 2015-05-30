
#include <ace.h>

// TextureObject2Dのサンプル。TextureObject2Dを用いて画像を表示しています。
void TextureObject2D()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("TextureObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	// テクスチャを読み込む
	auto tex = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());

	// TextureObject2Dのインスタンスを生成する
	auto obj = std::make_shared<ace::TextureObject2D>();

	// 描画に使うテクスチャを設定する
	obj->SetTexture(tex);

	// 描画位置を指定する
	obj->SetPosition(ace::Vector2DF(100, 100));

	// レイヤーへ追加する
	layer->AddObject(obj);

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}