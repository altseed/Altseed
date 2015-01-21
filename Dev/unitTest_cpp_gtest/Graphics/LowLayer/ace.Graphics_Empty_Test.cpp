
#include "../ace.Graphics_Test_Utls.h"

void Graphics_Empty(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"Empty").c_str());
	ASSERT_TRUE(window != nullptr);

	auto file = ace::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode ? ace::GraphicsDeviceType::OpenGL : ace::GraphicsDeviceType::DirectX11, nullptr, file, false, false);
	ASSERT_TRUE(graphics != nullptr);

	while (window->DoEvent())
	{
		graphics->Present();
		window->Close();
	}

	graphics->Release();
	file->Release();
	window->Release();
}

#ifdef _WIN32
TEST(Graphics, Empty_DX)
{
	Graphics_Empty(false);
}
#endif

TEST(Graphics, Empty_GL)
{
	Graphics_Empty(true);
}

