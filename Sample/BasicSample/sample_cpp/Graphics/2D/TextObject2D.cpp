
#include <Altseed.h>

// TextObject2Dのサンプル。TextObject2Dを用いて文字を表示しています。
void TextObject2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	// フォントを生成する
	auto smallFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/small.aff").c_str());

	// TextObject2Dのインスタンスを生成する
	auto smallObj = std::make_shared<asd::TextObject2D>();

	// 描画に使うフォントを設定する
	smallObj->SetFont(smallFont);

	// 描画位置を指定する
	smallObj->SetPosition(asd::Vector2DF(100, 100));

	// 描画する文字列を指定する
	smallObj->SetText(asd::ToAString("文字列サンプル1").c_str());

	//レイヤーへ追加する
	layer->AddObject(smallObj);

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