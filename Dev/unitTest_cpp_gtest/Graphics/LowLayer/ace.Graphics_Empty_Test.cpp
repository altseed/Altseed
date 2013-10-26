
#include "../ace.Graphics_Test_Utls.h"

void Graphics_Empty(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"Empty").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode, nullptr);
	ASSERT_TRUE(graphics != nullptr);

	while (window->DoEvent())
	{
		graphics->Present();
		window->Close();
	}

	graphics->Release();
	delete window;
}

TEST(Graphics, Empty_DX)
{
	Graphics_Empty(false);
}


TEST(Graphics, Empty_GL)
{
	Graphics_Empty(true);
}

