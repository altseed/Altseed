
#include <Altseed.h>

// Joystickのサンプル。ジョイスティックによる入力を取得しています。
void Joystick()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Joystick").c_str(), 640, 480, asd::EngineOption());

	// ジョイスティックが最低一つ接続されているかどうかを確認する。
	if (!asd::Engine::GetJoystickContainer()->GetIsPresentAt(0))
	{
		// 一つも接続されていない場合は終了する
		asd::Engine::Terminate();
		printf("No joystick are connected.\n");
		return;
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// 一つ目のジョイスティックの0番目のボタンの入力状態を表示する
		asd::Joystick* joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case asd::JoystickButtonState::Free: //前フレームと本フレームで非押下
			printf("Free\n");
			break;
		case asd::JoystickButtonState::Hold: //前フレームと本フレームで押下
			printf("Hold\n");
			break;
		case asd::JoystickButtonState::Release: //前フレームで押下、本フレームで非押下
			printf("Release\n");
			break;
		case asd::JoystickButtonState::Push: //前フレームで非押下、本フレームで押下
			printf("Push\n");
			break;
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
