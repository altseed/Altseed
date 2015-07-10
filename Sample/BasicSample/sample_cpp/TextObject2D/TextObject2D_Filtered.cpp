
#include <Base.h>

/**
@brief	文字列をギザギザを防いで回転させて表示するサンプル。
*/
void TextObject2D_Filtered()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("TextObject2D_Filtered").c_str(), 640, 480, asd::EngineOption());

	{
		// 動的フォントの取得と文字列描画オブジェクトの設定を行う。
		auto edgeFont = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 40, asd::Color(255, 128, 128, 255), 1, asd::Color(255, 255, 255, 255));
		auto edgeObj = std::make_shared<asd::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(asd::Vector2DF(100, 100));

		// 30度回転させて描画する。
		edgeObj->SetAngle(30);

		// 描画のフィルタを線形補間にすることによって、描画時に境界がギザギザにならないようにする。
		edgeObj->SetTextureFilterType(asd::TextureFilterType::Linear);

		edgeObj->SetText(asd::ToAString("ギザギザをなくした文字列の描画").c_str());

		// オブジェクトのインスタンスをエンジンへ追加する。
		asd::Engine::AddObject2D(edgeObj);
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