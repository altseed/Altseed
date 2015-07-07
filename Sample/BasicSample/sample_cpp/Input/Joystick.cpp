
#include <Altseed.h>

/**
@brief	Joystickのサンプル。ジョイスティックによる入力を取得しています。
*/
void Joystick()
{

	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Joystick").c_str(), 640, 480, asd::EngineOption());

	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	auto buttonsStateText = std::make_shared<asd::TextObject2D>();
	buttonsStateText->SetPosition(asd::Vector2DF(10, 10));
	buttonsStateText->SetFont(font);

	asd::Engine::AddObject2D(buttonsStateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		asd::astring displayStr = asd::ToAString("");

		// ジョイスティックが最低一つ接続されているかどうかを確認する。

		// 一つも接続されていない場合は終了する。
		if (!asd::Engine::GetJoystickContainer()->GetIsPresentAt(0))
		{
			displayStr += asd::ToAString("No joysticks are connected.\n");
		}
		else
		{
			// 一つ目のジョイスティックの0番目のボタンの入力状態を表示する
			asd::Joystick* joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);

			for (int buttonIndex = 0; buttonIndex < joystick->GetButtonsCount(); ++buttonIndex)
			{
				switch (joystick->GetButtonState(buttonIndex))
				{
				case asd::JoystickButtonState::Free: //前フレームと本フレームで非押下
					displayStr += asd::ToAString(("Button " + std::to_string(buttonIndex) + ": Free").c_str());
					break;
				case asd::JoystickButtonState::Hold: //前フレームと本フレームで押下
					displayStr += asd::ToAString(("Button " + std::to_string(buttonIndex) + ": Hold").c_str());
					break;
				case asd::JoystickButtonState::Release: //前フレームで押下、本フレームで非押下
					displayStr += asd::ToAString(("Button " + std::to_string(buttonIndex) + ": Release").c_str());
					break;
				case asd::JoystickButtonState::Push: //前フレームで非押下、本フレームで押下
					displayStr += asd::ToAString(("Button " + std::to_string(buttonIndex) + ": Push").c_str());
					break;
				}

				displayStr += asd::ToAString("\n");
			}

		}
		
		buttonsStateText->SetText(displayStr.c_str());

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
