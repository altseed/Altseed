
#include "../asd.Graphics_Test_Utls.h"

#include <Graphics/2D/asd.Renderer2D_Imp.h>

struct Vertex
{
	asd::Vector3DF Pos;
	asd::Vector2DF UV;

	Vertex() {}
	Vertex(asd::Vector3DF pos, asd::Vector2DF uv)
	{
		Pos = pos;
		UV = uv;
	}
};

void Graphics_Renderer2D(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	asd::Log* log = asd::Log_Imp::Create(u"graphics.html", u"描画");

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(u"Renderer2D").c_str(), log, asd::WindowPositionType::Default, isOpenGLMode? asd::GraphicsDeviceType::OpenGL: asd::GraphicsDeviceType::DirectX11, asd::ColorSpaceType::LinearSpace, false);
	ASSERT_TRUE(window != nullptr);

	auto file = asd::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	asd::GraphicsOption go;
	go.IsFullScreen = false;
	go.IsReloadingEnabled = false;
	go.ColorSpace = asd::ColorSpaceType::LinearSpace;
	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, log, file, go);
	ASSERT_TRUE(graphics != nullptr);

	auto texture = graphics->CreateTexture2D(asd::ToAString(u"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	auto renderer = new asd::Renderer2D_Imp(graphics, log);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, asd::Color(64, 32, 16, 255));

		asd::Vector2DF positions[4];
		asd::Color colors[4];
		asd::Vector2DF uvs[4];

		colors[0] = asd::Color(255, 255, 255, 255);
		colors[1] = asd::Color(255, 255, 255, 255);
		colors[2] = asd::Color(255, 255, 255, 255);
		colors[3] = asd::Color(255, 255, 255, 255);

		positions[0].X = 0;
		positions[0].Y = 0;
		positions[1].X = 128;
		positions[1].Y = 0;
		positions[2].X = 128;
		positions[2].Y = 128;
		positions[3].X = 0;
		positions[3].Y = 128;

		uvs[0].X = 0;
		uvs[0].Y = 0;
		uvs[1].X = 1;
		uvs[1].Y = 0;
		uvs[2].X = 1;
		uvs[2].Y = 1;
		uvs[3].X = 0;
		uvs[3].Y = 1;

		renderer->AddSprite(positions, colors, uvs, texture.get(), asd::AlphaBlendMode::Blend, 0);

		positions[0].X = 128 + 0;
		positions[0].Y = 128 + 0;
		positions[1].X = 128 + 128;
		positions[1].Y = 128 + 0;
		positions[2].X = 128 + 128;
		positions[2].Y = 128 + 128;
		positions[3].X = 128 + 0;
		positions[3].Y = 128 + 128;
		renderer->AddSprite(positions, colors, uvs, nullptr, asd::AlphaBlendMode::Blend, 0);

		positions[0].X = 256 + 0;
		positions[0].Y = 256 + 0;
		positions[1].X = 256 + 128;
		positions[1].Y = 256 + 0;
		positions[2].X = 256 + 128;
		positions[2].Y = 256 + 128;
		positions[3].X = 256 + 0;
		positions[3].Y = 256 + 128;
		colors[0].R = 0;
		colors[1].G = 0;
		colors[2].B = 0;
		colors[3].A = 0;

		renderer->AddSprite(positions, colors, uvs, nullptr, asd::AlphaBlendMode::Blend, 0);

		renderer->SetArea(asd::RectF(0, 0, 640, 480), 0);
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

	delete renderer;

	graphics->Release();
	file->Release();
	texture.reset();
	window->Release();

	delete log;
}

#ifdef _WIN32
TEST(Graphics, Renderer2D_DX)
{
	Graphics_Renderer2D(false);
}
#endif

TEST(Graphics, Renderer2D_GL)
{
	Graphics_Renderer2D(true);
}

