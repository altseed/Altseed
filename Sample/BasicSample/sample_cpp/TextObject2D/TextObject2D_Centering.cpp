
#include <Altseed.h>

/**
@brief	文字列を中央揃えで描画するサンプル
*/

void TextObject2D_Centering()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Centering").c_str(), 640, 480, asd::EngineOption());

	// フォントを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 35, asd::Color(255, 0, 0, 255), 1, asd::Color(255, 255, 255, 255));

	// 文字描画オブジェクトを生成する。
	auto obj = std::make_shared<asd::TextObject2D>();

	// 描画に使うフォントを設定する。
	obj->SetFont(font);

	//描画する文字列を設定する。
	auto str = asd::ToAString("中央揃えサンプル");
	obj->SetText(str.c_str());

	//文字列を描画したときの縦横の大きさを取得する。
	auto size = font->CalcTextureSize(str.c_str(), asd::WritingDirection::Horizontal);

	// 描画位置を指定する。
	auto pos = asd::Vector2DF(asd::Engine::GetWindowSize().X - size.X, asd::Engine::GetWindowSize().Y - size.Y);
	obj->SetPosition(pos / 2);

	// 文字描画オブジェクトのインスタンスをエンジンへ追加する。
	asd::Engine::AddObject2D(obj);


	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}