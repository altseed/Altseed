
#include <Base.h>

void Joystick()
{
	// aceを初期化する。
	ace::Engine::Initialize(L"Joystick", 640, 480, ace::EngineOption());

	// ジョイスティックが最低一つ接続されているかどうかを確認する。
	if (!ace::Engine::GetJoystickContainer()->IsPresentAt(0))
	{
		//一つも接続されていない場合は終了する
		printf("No joystick are connected.\n");
		system("pause");
		exit(0);
	}
	else
	{
		//接続されている場合は、一つ目のジョイスティックの名前を取得して表示する
		const ace::achar *name = ace::Engine::GetJoystickContainer()->GetJoystickAt(0)->GetJoystickName();
		wprintf(L"%s was found.\n", name);
	}

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{		
		//一つ目のジョイスティックの0番目のボタンの入力状態を表示する
		ace::Joystick* joystick = ace::Engine::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case ace::JoystickButtonState::Free:
			printf("FREE\n");
			break;
		case ace::JoystickButtonState::Hold:
			printf("HOLD\n");
			break;
		case ace::JoystickButtonState::Pull:
			printf("PULL\n");
			break;
		case ace::JoystickButtonState::Push:
			printf("PUSH\n");
			break;
		}

		// aceを更新する。
		ace::Engine::Update();
	}

	//aceを終了する。
	ace::Engine::Terminate();
}
