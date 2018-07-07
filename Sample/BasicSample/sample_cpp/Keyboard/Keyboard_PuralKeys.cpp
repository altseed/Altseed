
#include <Altseed.h>

/**
@brief	キーボードの複数のキーの入力状態を取得するサンプル。
*/
void Keyboard_PuralKeys()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Keyboard_PuralKeys").c_str(), 640, 480, asd::EngineOption());

	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 40, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// キーの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto keyStateText = std::make_shared<asd::TextObject2D>();
	keyStateText->SetPosition(asd::Vector2DF(10, 10));
	keyStateText->SetFont(font);
	asd::Engine::AddObject2D(keyStateText);

	// 入力チェックするキー一覧。(Z, X, C, V, B)
	std::vector<asd::Keys> keys;
	keys.push_back(asd::Keys::Z);
	keys.push_back(asd::Keys::X);
	keys.push_back(asd::Keys::C);
	keys.push_back(asd::Keys::V);
	keys.push_back(asd::Keys::B);

	//キーを文字列に変換するマップ。
	std::vector<std::string> keyStrs;
	keyStrs.push_back("Z");
	keyStrs.push_back("X");
	keyStrs.push_back("C");
	keyStrs.push_back("V");
	keyStrs.push_back("B");

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		asd::astring displayStr = asd::ToAString("");

		//すべてのキーに対して入力状態を確認してから表示する。
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string keystr = keyStrs[i];

			// キーボードのZキーの入力状態を取得する。
			asd::ButtonState zstate = asd::Engine::GetKeyboard()->GetKeyState(keys[i]);

			if (zstate == asd::ButtonState::Free) // 前フレームと本フレームで非押下
			{
				displayStr += asd::ToAString((keystr + "キーを離しています。").c_str());
			}
			else if (zstate == asd::ButtonState::Hold) // 前フレームと本フレームで押下
			{
				displayStr += asd::ToAString((keystr + "キーを押しています。").c_str());
			}
			else if (zstate == asd::ButtonState::Release) // 前フレームで押下、本フレームで非押下
			{
				displayStr += asd::ToAString((keystr + "キーを離しました!").c_str());
			}
			else if (zstate == asd::ButtonState::Push) // 前フレームで非押下、本フレームで押下
			{
				displayStr += asd::ToAString((keystr + "キーを押しました!").c_str());
			}

			displayStr += asd::ToAString("\n");
		}

		// キー入力状態を示す文字列を更新する。
		keyStateText->SetText(displayStr.c_str());

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
