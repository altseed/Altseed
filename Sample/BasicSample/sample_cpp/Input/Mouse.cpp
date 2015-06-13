
#include <Altseed.h>

// Mouseのサンプル。マウスによる入力を取得しています。
void Mouse()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Mouse").c_str(), 640, 480, asd::EngineOption());

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスへの参照を取得する。
		asd::Mouse* mouse = asd::Engine::GetMouse();

		// マウスカーソルの座標を取得して表示する。
		{
			asd::Vector2DF v = mouse->GetPosition();

			printf("Cursor Position:(%f,%f) ", v.X, v.Y);
		}

		// マウスの中央クリック状態を取得して表示する。
		{
			switch (mouse->GetMiddleButton()->GetButtonState())
			{
			case asd::MouseButtonState::Free: //前フレームと本フレームで非押下
				printf("Middle Free");
				break;
			case asd::MouseButtonState::Hold: //前フレームと本フレームで押下
				printf("Middle Hold");
				break;
			case asd::MouseButtonState::Release: //前フレームで押下、本フレームで非押下
				printf("Middle Release");
				break;
			case asd::MouseButtonState::Push: //前フレームで非押下、本フレームで押下
				printf("Middle Push");
				break;
			}
		}

		// マウスのホイールの回転状態を取得して表示する。
		{
			printf(" Wheel : %f\n", mouse->GetMiddleButton()->GetRotation());
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
