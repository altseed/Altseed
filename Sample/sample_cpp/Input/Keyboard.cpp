
#include <Base.h>

void Keyboard()
{
	// aceを初期化する。
	ace::Engine::Initialize(L"Keyboard", 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		//キーボードのZキーの入力状態を取得する。
		switch (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z))
		{
		case ace::KeyboardButtonState::Free: //前フレームと本フレームで非押下
			printf("Z key free\n");
			break;
		case ace::KeyboardButtonState::Hold: //前フレームと本フレームで押下
			printf("Z key hold\n");
			break;
		case ace::KeyboardButtonState::Pull: //前フレームで押下、本フレームで非押下
			printf("Z key pull\n");
			break;
		case ace::KeyboardButtonState::Push: //前フレームで非押下、本フレームで押下
			printf("Z key push\n");
			break;
		}

		//aceを更新する。
		ace::Engine::Update();
	}

	//aceを終了する。
	ace::Engine::Terminate();
}
