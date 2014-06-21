﻿
#include <Base.h>

void Joystick()
{
	ace::Engine::Initialize(L"Joystick", 640, 480, ace::EngineOption());

	if (!ace::Engine::GetJoystickContainer()->IsPresentAt(0))
	{
		printf("No joystick are connected.\n");
		system("pause");
		exit(0);
	}
	else
	{
		const ace::achar *name = ace::Engine::GetJoystickContainer()->GetJoystickAt(0)->GetJoystickName();
		wprintf(L"%s was found.\n", name);
	}

	while (ace::Engine::DoEvents())
	{		
		ace::Joystick* joystick = ace::Engine::GetJoystickContainer()->GetJoystickAt(0);
		switch (joystick->GetButtonState(0))
		{
		case ace::JoystickButtonState::Free:
			printf("FREE\n");
			break;
		case ace::JoystickButtonState::Hold:
			printf("HOLD\n");
			break;
		case ace::JoystickButtonState::Pull:
			printf("PULL\n");
			break;
		case ace::JoystickButtonState::Push:
			printf("PUSH\n");
			break;
		}

		ace::Engine::Update();
	}

	ace::Engine::Terminate();
}
