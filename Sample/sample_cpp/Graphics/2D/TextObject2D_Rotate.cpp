
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void TextObject2D_Rotate()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Rotate").c_str(), 640, 480, asd::EngineOption());

	{
		auto edgeFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/edge.aff").c_str());
		auto edgeObj = std::make_shared<asd::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(asd::Vector2DF(100, 300));

		//30度回転させて描画する
		edgeObj->SetAngle(30);
		edgeObj->SetText(asd::ToAString("文字列サンプル3").c_str());
		asd::Engine::AddObject2D(edgeObj);
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