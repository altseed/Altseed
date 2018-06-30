
#include "../asd.Graphics_Test_Utls.h"

#include <Graphics/2D/asd.LayerRenderer.h>


void Graphics_LayerRenderer(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	asd::Log* log = asd::Log_Imp::Create(u"graphics.html", u"レイヤー");

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(u"レイヤー").c_str(), log, false, asd::WindowPositionType::Default, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, asd::ColorSpaceType::LinearSpace, false);
	ASSERT_TRUE(window != nullptr);

	auto synchronizer = std::make_shared<asd::Synchronizer>();
	auto file = asd::File_Imp::Create(synchronizer);
	ASSERT_TRUE(file != nullptr);

	asd::GraphicsOption go;
	go.IsFullScreen = false;
	go.IsReloadingEnabled = false;
	go.ColorSpace = asd::ColorSpaceType::LinearSpace;
	go.GraphicsDevice = isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11;
	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, log, file, go);
	ASSERT_TRUE(graphics != nullptr);

	auto renderer = new asd::LayerRenderer(graphics);
	ASSERT_TRUE(renderer != nullptr);

	auto texture = graphics->CreateTexture2D(asd::ToAString(u"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, asd::Color(0, 0, 0, 255));

		{
			asd::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = 600;
			lpos[1].Y = 0;
			lpos[2].X = 640;
			lpos[2].Y = 480;
			lpos[3].X = 0;
			lpos[3].Y = 440;
			renderer->SetWindowSize(asd::Vector2DI(640, 480));
			renderer->SetTexture(texture.get());
			renderer->SetLayerPosition(lpos);
		}

		{
			asd::Vector2DF positions[4];
			asd::Color colors[4];
			asd::Vector2DF uvs[4];

			colors[0] = asd::Color(255, 255, 255, 255);
			colors[1] = asd::Color(255, 255, 255, 255);
			colors[2] = asd::Color(255, 255, 255, 255);

			positions[0].X = 0.0f;
			positions[0].Y = 0.0f;
			positions[1].X = 1.0f;
			positions[1].Y = 0.0f;
			positions[2].X = 1.0f;
			positions[2].Y = 1.0f;


			uvs[0].X = 0;
			uvs[0].Y = 0;
			uvs[1].X = 1;
			uvs[1].Y = 0;
			uvs[2].X = 1;
			uvs[2].Y = 1;

			renderer->AddTriangle(positions, colors, uvs);
		}

		{
			asd::Vector2DF positions[4];
			asd::Color colors[4];
			asd::Vector2DF uvs[4];

			colors[0] = asd::Color(255, 255, 255, 255);
			colors[1] = asd::Color(255, 255, 255, 255);
			colors[2] = asd::Color(255, 255, 255, 255);

			positions[0].X = 0.0f;
			positions[0].Y = 0.0f;
			positions[1].X = 0.5f;
			positions[1].Y = 1.0f;
			positions[2].X = 0.0f;
			positions[2].Y = 0.5f;

			uvs[0].X = 0;
			uvs[0].Y = 0;
			uvs[1].X = 1;
			uvs[1].Y = 1;
			uvs[2].X = 0;
			uvs[2].Y = 1;

			renderer->AddTriangle(positions, colors, uvs);
		}
		

		renderer->DrawCache();
		renderer->ClearCache();

		graphics->Present();

		graphics->End();

		if (time == 10)
		{
			SAVE_SCREEN_SHOT(graphics, 0);
		}

		if (time == 11)
		{
			window->Close();
		}
		time++;
	}

	texture.reset();
	renderer->Release();
	graphics->Release();
	file->Release();
	window->Release();
	delete log;
}

#ifdef _WIN32
TEST(Graphics, LayerRenderer_DX)
{
	Graphics_LayerRenderer(false);
}
#endif

TEST(Graphics, LayerRenderer_GL)
{
	Graphics_LayerRenderer(true);
}

