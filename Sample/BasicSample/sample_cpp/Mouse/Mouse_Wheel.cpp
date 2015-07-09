#include <Altseed.h>

/**
@brief	マウスのホイールの回転度合を取得するサンプル。
*/
void Mouse_Wheel()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Mouse_Wheel").c_str(), 640, 480, asd::EngineOption());

	// マウスの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 20, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// マウスのホイールの回転度合を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto wheelStateText = std::make_shared<asd::TextObject2D>();
	wheelStateText->SetPosition(asd::Vector2DF(10, 10));
	wheelStateText->SetFont(font);
	asd::Engine::AddObject2D(wheelStateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスのホイールの回転状態を取得して表示する。
		wheelStateText->SetText(asd::ToAString(("ホイールの回転度合 : " + std::to_string(asd::Engine::GetMouse()->GetMiddleButton()->GetRotation())).c_str()).c_str());

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
