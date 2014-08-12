
#include <Base.h>

void Mouse()
{
	//aceを初期化する。
	ace::Engine::Initialize(ace::ToAString("Mouse").c_str(), 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		//マウスへの参照を取得する。
		ace::Mouse* mouse = ace::Engine::GetMouse();

		//マウスカーソルの座標を取得して表示する。
		{
			ace::Vector2DF v = mouse->GetPosition();

			printf("Cursor Position:(%f,%f) ", v.X, v.Y);
		}

		//マウスの中央クリック状態を取得して表示する。
		{
			switch (mouse->GetMiddleButton()->GetButtonState())
			{
			case ace::MouseButtonState::Free: //前フレームと本フレームで非押下
				printf("Middle Free");
				break;
			case ace::MouseButtonState::Hold: //前フレームと本フレームで押下
				printf("Middle Hold");
				break;
			case ace::MouseButtonState::Release: //前フレームで押下、本フレームで非押下
				printf("Middle Release");
				break;
			case ace::MouseButtonState::Push: //前フレームで非押下、本フレームで押下
				printf("Middle Push");
				break;
			}
		}

		//マウスのホイールの回転状態を取得して表示する。
		{
			printf(" Wheel : %f\n", mouse->GetMiddleButton()->GetRotation());
		}

		//aceを更新する。
		ace::Engine::Update();
	}

	//aceを終了する。
	ace::Engine::Terminate();
}
