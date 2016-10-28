
#include <Altseed.h>

/**
@brief	ジョイスティックにある全てのボタンの入力状態を取得するサンプル。
*/
void Joystick_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(u"Joystick_Basic", 640, 480, asd::EngineOption());

	// ジョイスティックの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(u"", 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	// ボタンの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
	auto stateText = std::make_shared<asd::TextObject2D>();
	stateText->SetPosition(asd::Vector2DF(10, 5));
	stateText->SetFont(font);
	asd::Engine::AddObject2D(stateText);

	// ボタンをたくさん認識する可能性があるため行間を詰める。
	stateText->SetLineSpacing(-15);

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
			// 1つ目のジョイスティックの全てのボタンの入力状態を表示する。
			auto joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);

			for (int buttonIndex = 0; buttonIndex < joystick->GetButtonsCount(); ++buttonIndex)
			{
				auto state = joystick->GetButtonState(buttonIndex);
				if (state == asd::JoystickButtonState::Free) //前フレームと本フレームで非押下
				{
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を離しています。").c_str());
				}
				else if (state == asd::JoystickButtonState::Hold) //前フレームと本フレームで押下
				{
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を押しています。").c_str());
				}
				else if (state == asd::JoystickButtonState::Release) //前フレームで押下、本フレームで非押下
				{
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を離しました!").c_str());
				}
				else if (state == asd::JoystickButtonState::Push) //前フレームで非押下、本フレームで押下
				{
					displayStr += asd::ToAString(("ボタン " + std::to_string(buttonIndex) + "を押しました!").c_str());
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
