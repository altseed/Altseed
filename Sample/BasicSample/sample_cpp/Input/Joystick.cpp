
#include <ace.h>

// Joystickのサンプル。ジョイスティックによる入力を取得しています。
void Joystick()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Joystick").c_str(), 640, 480, ace::EngineOption());

	// ジョイスティックが最低一つ接続されているかどうかを確認する。
	if (!ace::Engine::GetJoystickContainer()->GetIsPresentAt(0))
	{
		// 一つも接続されていない場合は終了する
		ace::Engine::Terminate();
		printf("No joystick are connected.\n");
		return;
	}

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// 一つ目のジョイスティックの0番目のボタンの入力状態を表示する
		ace::Joystick* joystick = ace::Engine::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case ace::JoystickButtonState::Free: //前フレームと本フレームで非押下
			printf("Free\n");
			break;
		case ace::JoystickButtonState::Hold: //前フレームと本フレームで押下
			printf("Hold\n");
			break;
		case ace::JoystickButtonState::Release: //前フレームで押下、本フレームで非押下
			printf("Release\n");
			break;
		case ace::JoystickButtonState::Push: //前フレームで非押下、本フレームで押下
			printf("Push\n");
			break;
		}

		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	//AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}
