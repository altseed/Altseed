
#include <Base.h>

void Keyboard()
{
	// aceを初期化する。
	asd::Engine::Initialize(asd::ToAString("Keyboard").c_str(), 640, 480, asd::EngineOption());

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		//キーボードのZキーの入力状態を取得する。
		switch (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Z))
		{
		case asd::KeyState::Free: //前フレームと本フレームで非押下
			printf("Z key free\n");
			break;
		case asd::KeyState::Hold: //前フレームと本フレームで押下
			printf("Z key hold\n");
			break;
		case asd::KeyState::Release: //前フレームで押下、本フレームで非押下
			printf("Z key release\n");
			break;
		case asd::KeyState::Push: //前フレームで非押下、本フレームで押下
			printf("Z key push\n");
			break;
		}

		//aceを更新する。
		asd::Engine::Update();
	}

	//aceを終了する。
	asd::Engine::Terminate();
}
