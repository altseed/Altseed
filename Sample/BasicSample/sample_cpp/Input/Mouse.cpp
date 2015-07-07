#include <Altseed.h>

/**
@brief	Mouseのサンプル。マウスによる入力を取得しています。
*/
void Mouse()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Mouse").c_str(), 640, 480, asd::EngineOption());

	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 20, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	auto mousePosText = std::make_shared<asd::TextObject2D>();
	mousePosText->SetPosition(asd::Vector2DF(100, 100));
	mousePosText->SetFont(font);

	auto wheelStateText = std::make_shared<asd::TextObject2D>();
	wheelStateText->SetPosition(asd::Vector2DF(100, 300));
	wheelStateText->SetFont(font);

	auto leftButtonStateText = std::make_shared<asd::TextObject2D>();
	leftButtonStateText->SetPosition(asd::Vector2DF(100, 200));
	leftButtonStateText->SetFont(font);

	asd::Engine::AddObject2D(mousePosText);
	asd::Engine::AddObject2D(wheelStateText);
	asd::Engine::AddObject2D(leftButtonStateText);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスへの参照を取得する。
		asd::Mouse* mouse = asd::Engine::GetMouse();

		// マウスカーソルの座標を取得して表示する。
		{
			asd::Vector2DF v = mouse->GetPosition();

			mousePosText->SetText(asd::ToAString(("Cursor Position : (" + std::to_string((int)(v.X)) + "," + std::to_string((int)(v.Y)) + ")").c_str()).c_str());
		}

		// マウスの中央クリック状態を取得して表示する。
		{
			switch (mouse->GetMiddleButton()->GetButtonState())
			{
			case asd::MouseButtonState::Free: //前フレームと本フレームで非押下
				leftButtonStateText->SetText(asd::ToAString("Left Button : Free").c_str());
				break;
			case asd::MouseButtonState::Hold: //前フレームと本フレームで押下
				leftButtonStateText->SetText(asd::ToAString("Left Button : Hold").c_str());
				break;
			case asd::MouseButtonState::Release: //前フレームで押下、本フレームで非押下
				leftButtonStateText->SetText(asd::ToAString("Left Button : Release").c_str());
				break;
			case asd::MouseButtonState::Push: //前フレームで非押下、本フレームで押下
				leftButtonStateText->SetText(asd::ToAString("Left Button : Push").c_str());
				break;
			}
		}

		// マウスのホイールの回転状態を取得して表示する。
		{
			wheelStateText->SetText(asd::ToAString(("Wheel : " + std::to_string(mouse->GetMiddleButton()->GetRotation())).c_str()).c_str());
		}
		
		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
