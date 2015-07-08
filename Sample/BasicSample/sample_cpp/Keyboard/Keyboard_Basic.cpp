
#include <Altseed.h>

// キーボードによる入力を取得するサンプル。
void Keyboard_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Keyboard_Basic").c_str(), 640, 480, asd::EngineOption());

	// 入力チェックするキー一覧。(Z, X, C, V, B)
	std::vector<asd::Keys> keys = { asd::Keys::Z, asd::Keys::X, asd::Keys::C, asd::Keys::V, asd::Keys::B };

	//キーを文字列に変換するマップ。
	std::map<asd::Keys, std::string> keyToStr = { 
		{ asd::Keys::Z, "Z" },
		{ asd::Keys::X, "X" },
		{ asd::Keys::C, "C" },
		{ asd::Keys::V, "V" },
		{ asd::Keys::B, "B" }
	};

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		asd::astring displayStr = asd::ToAString("");

		//すべてのキーに対して入力状態を確認してから表示する。
		for (auto key : keys)
		{
			auto keyStr = keyToStr[key];

			// キーボードのZキーの入力状態を取得する。
			switch (asd::Engine::GetKeyboard()->GetKeyState(key))
			{
			case asd::KeyState::Free: // 前フレームと本フレームで非押下
				displayStr += asd::ToAString((keyStr + "キーを離しています。").c_str());
				break;
			case asd::KeyState::Hold: // 前フレームと本フレームで押下
				displayStr += asd::ToAString((keyStr + "キーを押しています。").c_str());
				break;
			case asd::KeyState::Release: // 前フレームで押下、本フレームで非押下
				displayStr += asd::ToAString((keyStr + "キーを離しました!").c_str());
				break;
			case asd::KeyState::Push: // 前フレームで非押下、本フレームで押下
				displayStr += asd::ToAString((keyStr + "キーを押しました!").c_str());
				break;
			}

			displayStr += asd::ToAString("\n");
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
