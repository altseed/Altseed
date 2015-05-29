
#include <ace.h>

void Keyboard()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Keyboard").c_str(), 640, 480, ace::EngineOption());

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		//キーボードのZキーの入力状態を取得する。
		switch (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z))
		{
		case ace::KeyState::Free: //前フレームと本フレームで非押下
			printf("Z key free\n");
			break;
		case ace::KeyState::Hold: //前フレームと本フレームで押下
			printf("Z key hold\n");
			break;
		case ace::KeyState::Release: //前フレームで押下、本フレームで非押下
			printf("Z key release\n");
			break;
		case ace::KeyState::Push: //前フレームで非押下、本フレームで押下
			printf("Z key push\n");
			break;
		}

		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	//AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}
