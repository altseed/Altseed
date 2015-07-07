#include <Altseed.h>

// マウスによる入力を取得を取得するサンプル。
void Mouse_Click()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Mouse_Click").c_str(), 640, 480, asd::EngineOption());

	// マウスの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	auto stateText = std::make_shared<asd::TextObject2D>();
	stateText->SetPosition(asd::Vector2DF(10, 10));
	stateText->SetFont(font);

	asd::Engine::AddObject2D(stateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// 左ボタンが押されているかを表示する。
		if (asd::Engine::GetMouse()->GetLeftButton()->GetButtonState() == asd::MouseButtonState::Hold)
		{
			stateText->SetText(asd::ToAString("左ボタンが押されています。").c_str());
		}
		else
		{
			stateText->SetText(asd::ToAString("左ボタンが押されていません。").c_str());
		}
		
		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
