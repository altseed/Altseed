
#include <Altseed.h>

// キーボードによる入力を取得するサンプル。
void Keyboard_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Keyboard_Basic").c_str(), 640, 480, asd::EngineOption());

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// キーボードのZキーの入力状態を取得する。
		switch (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Z))
		{
		case asd::KeyState::Free: // 前フレームと本フレームで非押下
			printf("Z key free\n");
			break;
		case asd::KeyState::Hold: // 前フレームと本フレームで押下
			printf("Z key hold\n");
			break;
		case asd::KeyState::Release: // 前フレームで押下、本フレームで非押下
			printf("Z key release\n");
			break;
		case asd::KeyState::Push: // 前フレームで非押下、本フレームで押下
			printf("Z key push\n");
			break;
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
