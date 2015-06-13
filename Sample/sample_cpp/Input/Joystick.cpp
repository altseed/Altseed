
#include <Base.h>

void Joystick()
{
	// aceを初期化する。
	asd::Engine::Initialize(asd::ToAString("Joystick").c_str(), 640, 480, asd::EngineOption());

	// ジョイスティックが最低一つ接続されているかどうかを確認する。
	if (!asd::Engine::GetJoystickContainer()->GetIsPresentAt(0))
	{
		//一つも接続されていない場合は終了する
		printf("No joystick are connected.\n");
		system("pause");
		exit(0);
	}
	else
	{
		//接続されている場合は、一つ目のジョイスティックの名前を取得して表示する
		const asd::achar *name = asd::Engine::GetJoystickContainer()->GetJoystickAt(0)->GetJoystickName();
		wprintf(L"%s was found.\n", name);
	}

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		//一つ目のジョイスティックの0番目のボタンの入力状態を表示する
		asd::Joystick* joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case asd::JoystickButtonState::Free: //前フレームと本フレームで非押下
			printf("FREE\n");
			break;
		case asd::JoystickButtonState::Hold: //前フレームと本フレームで押下
			printf("HOLD\n");
			break;
		case asd::JoystickButtonState::Release: //前フレームで押下、本フレームで非押下
			printf("RELEASE\n");
			break;
		case asd::JoystickButtonState::Push: //前フレームで非押下、本フレームで押下
			printf("PUSH\n");
			break;
		}

		// aceを更新する。
		asd::Engine::Update();
	}

	//aceを終了する。
	asd::Engine::Terminate();
}
