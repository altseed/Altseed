
#include <Altseed.h>

/**
@brief	キーボードによる入力を取得するサンプル。
*/
void Keyboard_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Keyboard_Basic").c_str(), 640, 480, asd::EngineOption());

	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 40, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// キーの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto keyStateText = std::make_shared<asd::TextObject2D>();
	keyStateText->SetPosition(asd::Vector2DF(10, 10));
	keyStateText->SetFont(font);
	asd::Engine::AddObject2D(keyStateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{

		// キーボードのZキーの入力状態を取得する。
		auto zstate = asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Z);

		if (zstate == asd::KeyState::Free) // Zキーを離している状態。
		{
			keyStateText->SetText(asd::ToAString("Zキーを離しています。").c_str());
		}
		else if (zstate == asd::KeyState::Hold) // Zキーを押している状態。
		{
			keyStateText->SetText(asd::ToAString("Zキーを押しています。").c_str());
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
