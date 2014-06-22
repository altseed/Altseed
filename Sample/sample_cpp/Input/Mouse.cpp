
#include <Base.h>

static void GetPositionTest(ace::Mouse* mouse)
{
	ace::Vector2DF v = mouse->GetPosition();

	printf("Cursor Position:(%f,%f)\n", v.X, v.Y);
}

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

static void WheelTest(ace::Mouse *mouse)
{
	printf("Wheel : %f\n", mouse->GetMiddleButton()->GetRotation());
}

void Mouse()
{
	ace::Engine::Initialize(L"Mouse", 640, 480, ace::EngineOption());

	while (ace::Engine::DoEvents())
	{
		ace::Mouse* mouse = ace::Engine::GetMouse();

		WheelTest(mouse);

		ace::Engine::Update();
	}

	ace::Engine::Terminate();
}
