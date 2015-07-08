
#include <Altseed.h>

// ジョイスティックによる入力を取得するサンプル。

void Joystick_Axis()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Joystick_Axis").c_str(), 640, 480, asd::EngineOption());

	// ジョイスティックの状態を表示するテキストを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 25, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

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
			// 1つ目のジョイスティックの全てのアナログスティックの入力状態を表示する
			asd::Joystick* joystick = asd::Engine::GetJoystickContainer()->GetJoystickAt(0);

			for (int axisIndex = 0; axisIndex < joystick->GetAxesCount(); ++axisIndex)
			{
				auto axisVal = joystick->GetAxisState(axisIndex);
				displayStr += asd::ToAString(("軸 " + std::to_string(axisIndex) + ": ").c_str());
				displayStr += asd::ToAString(std::to_string(axisVal).c_str());
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
