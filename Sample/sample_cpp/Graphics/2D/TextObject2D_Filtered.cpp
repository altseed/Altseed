
#include <Base.h>

/**
@brief	文字列をギザギザを防いで回転させて表示する。
*/
void TextObject2D_Filtered()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Filtered").c_str(), 640, 480, asd::EngineOption());

	{
		auto edgeFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/edge.aff").c_str());
		auto edgeObj = std::make_shared<asd::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(asd::Vector2DF(100, 100));

		// 30度回転させて描画する。
		edgeObj->SetAngle(30);

		// 描画のフィルタを線形補間にすることによって、描画時に境界がギザギザにならないようにする。
		edgeObj->SetTextureFilterType(asd::TextureFilterType::Linear);

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