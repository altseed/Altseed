
#include <Base.h>

/**
@brief	文字列を表示するサンプル。
*/
void TextObject2D_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Basic").c_str(), 640, 480, asd::EngineOption());

	{
		// フォントを生成する。
		auto font = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/Font1.aff").c_str());

		// 文字描画オブジェクトを生成する
		auto obj = std::make_shared<asd::TextObject2D>();

		// 描画に使うフォントを設定する。
		obj->SetFont(font);

		// 描画位置を指定する。
		obj->SetPosition(asd::Vector2DF(100, 100));

		// 描画する文字列を指定する。
		obj->SetText(asd::ToAString("普通の文字列描画").c_str());

		// 文字描画オブジェクトのインスタンスをエンジンへ追加する。
		asd::Engine::AddObject2D(obj);
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}