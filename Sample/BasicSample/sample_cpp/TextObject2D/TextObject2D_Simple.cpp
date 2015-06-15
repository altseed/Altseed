
#include <Base.h>

/**
@brief	文字列を表示するサンプル。
*/
void TextObject2D_Simple()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Simple").c_str(), 640, 480, asd::EngineOption());

	{
		// フォントを生成する
		auto font = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/edge.aff").c_str());

		//TextObject2Dを生成する
		auto obj = std::make_shared<asd::TextObject2D>();

		//描画に使うフォントを設定する
		obj->SetFont(font);

		//描画位置を指定する
		obj->SetPosition(asd::Vector2DF(100, 100));

		//描画する文字列の指定
		obj->SetText(asd::ToAString("文字列サンプル1").c_str());

		//レイヤーへ追加する
		asd::Engine::AddObject2D(obj);
	}

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}