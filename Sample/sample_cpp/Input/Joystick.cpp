#include <Base.h>

void Joystick()
{
	ace::GetEngine()->Initialize(L"Joystick", 640, 480, ace::EngineOption());

	if (!ace::GetJoystickContainer()->IsPresentAt(0))
	{
		printf("No joystick are connected.\n");
		system("pause");
		exit(0);
	}
	else
	{
		const ace::achar *name = ace::GetJoystickContainer()->GetJoystickAt(0)->GetJoystickName();
		wprintf(L"%s was found.\n", name);
	}

	while (ace::GetEngine()->DoEvents())
	{
		
		ace::Joystick* joystick = ace::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case ace::ACE_JOYSTICK_BUTTON_FREE:
			printf("FREE\n");
			break;
		case ace::ACE_JOYSTICK_BUTTON_HOLD:
			printf("HOLD\n");
			break;
		case ace::ACE_JOYSTICK_BUTTON_PULL:
			printf("PULL\n");
			break;
		case ace::ACE_JOYSTICK_BUTTON_PUSH:
			printf("PUSH\n");
			break;
		}

		ace::Sleep(50);
		
		
		ace::GetEngine()->Update();
	}

	ace::GetEngine()->Terminate();
}
