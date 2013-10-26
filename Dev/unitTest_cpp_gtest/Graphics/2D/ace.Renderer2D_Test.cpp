
#include "../ace.Graphics_Test_Utls.h"

#include <Graphics/Common/2D/ace.Renderer2D_Imp.h>

struct Vertex
{
	ace::Vector3DF Pos;
	ace::Vector2DF UV;

	Vertex() {}
	Vertex(ace::Vector3DF pos, ace::Vector2DF uv)
	{
		Pos = pos;
		UV = uv;
	}
};

void Graphics_Renderer2D(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"描画");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"Renderer2D").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode, log);
	ASSERT_TRUE(graphics != nullptr);

	auto texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	auto renderer = new ace::Renderer2D_Imp(graphics, log, window->GetSize());

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, ace::Color(64, 32, 16, 255));

		ace::Vector2DF positions[4];
		ace::Color colors[4];
		ace::Vector2DF uvs[4];

		colors[0] = ace::Color(255, 255, 255, 255);
		colors[1] = ace::Color(255, 255, 255, 255);
		colors[2] = ace::Color(255, 255, 255, 255);
		colors[3] = ace::Color(255, 255, 255, 255);

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

		renderer->AddSprite(positions, colors, uvs, texture.get(), ace::eAlphaBlend::ALPHA_BLEND_BLEND, 0);

		positions[0].X = 128 + 0;
		positions[0].Y = 128 + 0;
		positions[1].X = 128 + 128;
		positions[1].Y = 128 + 0;
		positions[2].X = 128 + 128;
		positions[2].Y = 128 + 128;
		positions[3].X = 128 + 0;
		positions[3].Y = 128 + 128;
		renderer->AddSprite(positions, colors, uvs, nullptr, ace::eAlphaBlend::ALPHA_BLEND_BLEND, 0);

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

		renderer->AddSprite(positions, colors, uvs, nullptr, ace::eAlphaBlend::ALPHA_BLEND_BLEND, 0);

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
	texture.reset();

	delete window;
	delete log;
}

TEST(Graphics, Renderer2D_DX)
{
	Graphics_Renderer2D(false);
}

TEST(Graphics, Renderer2D_GL)
{
	Graphics_Renderer2D(true);
}

