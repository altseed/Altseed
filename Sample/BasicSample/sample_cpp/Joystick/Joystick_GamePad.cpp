
#include <Altseed.h>

/**
@brief	一部のゲームパッドから入力状態を取得するサンプル。
*/
void Joystick_GamePad()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(u"Joystick_GamePad", 640, 480, asd::EngineOption());

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
			displayStr += u"ジョイスティックが接続されていません。";
		}
		else if (asd::Engine::GetJoystickContainer()->GetJoystickAt(0)->GetJoystickType() == asd::JoystickType::Other)
		{
			displayStr += u"認識できないジョイステイックです。";
		}
		else
		{
			// 1つ目のジョイスティックのボタンの入力状態を表示する。
			// RightRightはPSの場合〇、XBOXの場合、Bボタンを示す。
			auto joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);

			auto state = joystick->GetButtonStateAt(asd::JoystickButtonType::RightRight);
			if (state == asd::JoystickButtonState::Free) //前フレームと本フレームで非押下
			{
				displayStr += u"ボタンを離しています。";
			}
			else if (state == asd::JoystickButtonState::Hold) //前フレームと本フレームで押下
			{
				displayStr += u"ボタンを押しています。";
			}
			else if (state == asd::JoystickButtonState::Release) //前フレームで押下、本フレームで非押下
			{
				displayStr += u"ボタンを離しました!";
			}
			else if (state == asd::JoystickButtonState::Push) //前フレームで非押下、本フレームで押下
			{
				displayStr += u"ボタンを押しました!";
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
