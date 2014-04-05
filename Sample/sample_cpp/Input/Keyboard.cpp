
#include <Base.h>

void Keyboard()
{
	ace::Engine::Initialize(L"Keyboard", 640, 480, ace::EngineOption());

	while (ace::Engine::DoEvents())
	{
		switch (ace::Engine::GetKeyboard()->GetKeyState(ace::eKeys::ACE_KEY_Z))
		{
		case ace::KEYBOARD_FREE:
			printf("Z key free\n");
			break;
		case ace::KEYBOARD_HOLD:
			printf("Z key hold\n");
			break;
		case ace::KEYBOARD_PULL:
			printf("Z key pull\n");
			break;
		case ace::KEYBOARD_PUSH:
			printf("Z key push\n");
			break;
		}
		ace::Engine::Update();
	}

	ace::Engine::Terminate();
}
