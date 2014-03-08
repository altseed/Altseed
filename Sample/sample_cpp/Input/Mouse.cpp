
#include <Base.h>

static void GetPositionTest(ace::Mouse* mouse)
{
	ace::Vector2DF v = mouse->GetPosition();

	printf("Cursor Position:(%f,%f)\n", v.X, v.Y);
}

static void ClickTest(std::string title, const ace::eMouseButtonState state)
{
	switch (state)
	{
	case ace::eMouseButtonState::MOUSE_FREE:
		printf("State : %s : Free\n",title.c_str());
		break;
	case ace::eMouseButtonState::MOUSE_HOLD:
		printf("State : %s : Hold\n", title.c_str());
		break;
	case ace::eMouseButtonState::MOUSE_PULL:
		printf("State : %s : Pull\n", title.c_str());
		break;
	case ace::eMouseButtonState::MOUSE_PUSH:
		printf("State : %s : Push\n", title.c_str());
		break;
	}
	ace::Sleep(100);
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
