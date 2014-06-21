
#include <Base.h>

void Keyboard()
{
	ace::Engine::Initialize(L"Keyboard", 640, 480, ace::EngineOption());

	while (ace::Engine::DoEvents())
	{
		switch (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z))
		{
		case ace::KeyboardButtonState::Free:
			printf("Z key free\n");
			break;
		case ace::KeyboardButtonState::Hold:
			printf("Z key hold\n");
			break;
		case ace::KeyboardButtonState::Pull:
			printf("Z key pull\n");
			break;
		case ace::KeyboardButtonState::Push:
			printf("Z key push\n");
			break;
		}
		ace::Engine::Update();
	}

	ace::Engine::Terminate();
}
