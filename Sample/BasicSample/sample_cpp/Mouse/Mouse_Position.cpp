#include <Altseed.h>

/**
@brief	マウスの座標を取得を取得するサンプル。
*/
void Mouse_Position()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Mouse_Position").c_str(), 640, 480, asd::EngineOption());

	// マウスの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// マウスカーソルの座標を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto stateText = std::make_shared<asd::TextObject2D>();
	stateText->SetPosition(asd::Vector2DF(10, 10));
	stateText->SetFont(font);
	asd::Engine::AddObject2D(stateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスカーソルの座標を取得して表示する。
		asd::Vector2DF pos = asd::Engine::GetMouse()->GetPosition();
		stateText->SetText(asd::ToAString(("マウスカーソルの位置 : (" + std::to_string(pos.X) + "," + std::to_string(pos.Y) + ")").c_str()).c_str());
		
		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
