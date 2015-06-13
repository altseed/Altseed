
#include "../asd.Graphics_Test_Utls.h"

void Graphics_Empty(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(L"Empty").c_str());
	ASSERT_TRUE(window != nullptr);

	auto file = asd::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, nullptr, file, false, false);
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

