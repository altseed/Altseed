
#include <Base.h>

//マウスカーソルの座標を取得して表示する。
static void GetPositionTest(ace::Mouse* mouse)
{
	ace::Vector2DF v = mouse->GetPosition();

	printf("Cursor Position:(%f,%f)\n", v.X, v.Y);
}

//マウスのクリック状態を取得して表示する。
static void ClickTest(std::string title, const ace::MouseButtonState state)
{
	switch (state)
	{
	case ace::MouseButtonState::Free:
		printf("State : %s : Free\n",title.c_str());
		break;
	case ace::MouseButtonState::Hold:
		printf("State : %s : Hold\n", title.c_str());
		break;
	case ace::MouseButtonState::Pull:
		printf("State : %s : Pull\n", title.c_str());
		break;
	case ace::MouseButtonState::Push:
		printf("State : %s : Push\n", title.c_str());
		break;
	}
}

//マウスのホイールの回転状態を取得して表示する。
static void WheelTest(ace::Mouse *mouse)
{
	printf("Wheel : %f\n", mouse->GetMiddleButton()->GetRotation());
}

void Mouse()
{
	//aceを初期化する。
	ace::Engine::Initialize(L"Mouse", 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		//マウスへの参照を取得する。
		ace::Mouse* mouse = ace::Engine::GetMouse();

		WheelTest(mouse);

		//aceを更新する。
		ace::Engine::Update();
	}

	//aceを終了する。
	ace::Engine::Terminate();
}
