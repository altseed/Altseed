
#include <Altseed.h>

/**
@brief	ジョイスティックにある全てのボタンの入力状態を取得するサンプル。
*/
void Joystick_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Joystick_Basic").c_str(), 640, 480, asd::EngineOption());

	// ジョイスティックの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// ボタンの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto stateText = std::make_shared<asd::TextObject2D>();
	stateText->SetPosition(asd::Vector2DF(10, 10));
	stateText->SetFont(font);
	asd::Engine::AddObject2D(stateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		asd::astring displayStr = asd::ToAString("");

		// ジョイスティックが接続されているかどうかを確認する。
		if (!asd::Engine::GetJoystickContainer()->GetIsPresentAt(0))
		{
			displayStr += asd::ToAString("ジョイスティックが接続されていません。");
		}
		else
		{
			// 1つ目のジョイスティックの全てのボタンの入力状態を表示する
			auto joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);

			for (int buttonIndex = 0; buttonIndex < joystick->GetButtonsCount(); ++buttonIndex)
			{
				switch (joystick->GetButtonState(buttonIndex))
				{
				case asd::JoystickButtonState::Free: //前フレームと本フレームで非押下
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を離しています。").c_str());
					break;
				case asd::JoystickButtonState::Hold: //前フレームと本フレームで押下
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を押しています。").c_str());
					break;
				case asd::JoystickButtonState::Release: //前フレームで押下、本フレームで非押下
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を離しました!").c_str());
					break;
				case asd::JoystickButtonState::Push: //前フレームで非押下、本フレームで押下
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を押しました!").c_str());
					break;
				}

				displayStr += asd::ToAString("\n");
			}

		}
		
		stateText->SetText(displayStr.c_str());

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
