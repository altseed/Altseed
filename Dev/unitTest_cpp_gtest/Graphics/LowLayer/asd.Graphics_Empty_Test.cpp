
#include "../asd.Graphics_Test_Utls.h"

void Graphics_Empty(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(u"Empty").c_str(), NULL, asd::WindowPositionType::Default, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, asd::ColorSpaceType::LinearSpace, false);
	ASSERT_TRUE(window != nullptr);

	auto file = asd::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	asd::GraphicsOption go;
	go.IsFullScreen = false;
	go.IsReloadingEnabled = false;
	go.ColorSpace = asd::ColorSpaceType::LinearSpace;
	go.GraphicsDevice = isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11;
	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, nullptr, file, go);
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

