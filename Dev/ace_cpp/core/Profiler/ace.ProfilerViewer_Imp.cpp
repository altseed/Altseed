#include "ace.ProfilerViewer_Imp.h"

#include "../Graphics/ace.Graphics_Imp.h"

#include "../Graphics/Resource/ace.Texture2D_Imp.h"
#include "../Graphics/Resource/ace.Font_Imp.h"
#include "../Graphics/Resource/ace.Texture2D_Imp.h"

#include "../Graphics/2D/ace.Renderer2D_Imp.h"
#include "../Log/ace.Log_Imp.h"
#include <cmath>

#include "ace.InternalFont.h"
#include "ace.InternalFontTex0.h"
#include "ace.InternalFontTex1.h"

namespace ace
{
	const int GLOBAL_INFO_TOP = 20;
	const int MATER_HEIGHT = 20;
	const int TOP_OFFSET = 40;
	const int ID_LEFT_OFFSET = 30;
	const int TIME_LEFT_OFFSET = 50;
	const int MATER_LEFT_OFFSET = 20;

	std::shared_ptr<Font> ProfilerViewer_Imp::CreateFont_()
	{
		auto tex0 = (Texture2D*)m_graphics->CreateTexture2D_Imp_Internal(m_graphics, (uint8_t*)g_internalFontTex0, sizeof(g_internalFontTex0));
		auto tex1 = (Texture2D*)m_graphics->CreateTexture2D_Imp_Internal(m_graphics, (uint8_t*)g_internalFontTex1, sizeof(g_internalFontTex1));

		std::vector<std::shared_ptr<Texture2D>> textures;
		textures.push_back(CreateSharedPtrWithReleaseDLL(tex0));
		textures.push_back(CreateSharedPtrWithReleaseDLL(tex1));

		auto font = new Font_Imp(m_graphics, (void*)g_internalFont, sizeof(g_internalFont), textures);

		return CreateSharedPtrWithReleaseDLL((Font*)font);
	}

	void ProfilerViewer_Imp::GenerateRenderer()
	{
		if (isRendererGenerated) return;

		m_renderer = new Renderer2D_Imp(m_graphics, log);
		m_font = CreateFont_();

		isRendererGenerated = true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp::ProfilerViewer_Imp(
		Graphics_Imp* graphics,
		Log* log,
		Profiler_Imp* profiler,
		Core* core,
		Vector2DI windowSize)
		: m_profiler(profiler)
		, m_renderer(nullptr)
		, m_windowSize(windowSize)
		, m_graphics(graphics)
		, m_core(core)
		, m_materTexture(nullptr)
		, log(log)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp::~ProfilerViewer_Imp()
	{
		SafeDelete(m_renderer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp* ProfilerViewer_Imp::Create(
		Core* core,
		Profiler_Imp* profiler,
		Graphics_Imp* graphics,
		Log* logger,
		Vector2DI windowSize)
	{
		logger->WriteHeading("プロファイラビュアー");

		auto temp = new ProfilerViewer_Imp(graphics, logger, profiler, core, windowSize);

		logger->WriteLine("初期化成功");

		return temp;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void ProfilerViewer_Imp::SetProfiler(Profiler_Imp* profiler)
	{
		m_profiler = profiler;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void ProfilerViewer_Imp::Draw()
	{
		GenerateRenderer();

		AddBackgroundSprite(m_renderer);
		AddFpsSprite(m_renderer, m_core);
		AddDrawCallSprite(m_renderer, m_graphics->GetDrawCallCount());

		int index = 0;
		for (auto& profile : m_profiler->GetProfiles())
		{
			auto perf = profile->GetLastLog();
			if (perf == nullptr)
			{
				continue;
			}

			int time = perf->GetEndTime() - perf->GetStartTime();

			AddMaterSprite(m_renderer, index, time);
			AddIdSprite(m_renderer, index, profile->GetID());
			AddTimeSprite(m_renderer, index, time);

			++index;
		}

		m_renderer->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
		m_renderer->DrawCache();
		m_renderer->ClearCache();
	}

	void ProfilerViewer_Imp::AddBackgroundSprite(Renderer2D* renderer)
	{
		Sprite sprite;
		float left = 0;
		float top = 0;
		float right = m_windowSize.X;
		float bottom = m_windowSize.Y;

		sprite.positions[0].X = left;
		sprite.positions[0].Y = top;
		sprite.positions[1].X = right;
		sprite.positions[1].Y = top;
		sprite.positions[2].X = right;
		sprite.positions[2].Y = bottom;
		sprite.positions[3].X = left;
		sprite.positions[3].Y = bottom;

		sprite.uvs[0].X = 0;
		sprite.uvs[0].Y = 0;
		sprite.uvs[1].X = 1;
		sprite.uvs[1].Y = 0;
		sprite.uvs[2].X = 1;
		sprite.uvs[2].Y = 1;
		sprite.uvs[3].X = 0;
		sprite.uvs[3].Y = 1;

		sprite.colors[0] = Color(0, 0, 0, 128);
		sprite.colors[1] = Color(0, 0, 0, 128);
		sprite.colors[2] = Color(0, 0, 0, 128);
		sprite.colors[3] = Color(0, 0, 0, 128);

		renderer->AddSprite(
			sprite.positions,
			sprite.colors,
			sprite.uvs,
			m_materTexture.get(),
			AlphaBlend::Blend,
			0);
	}

	void ProfilerViewer_Imp::AddFpsSprite(Renderer2D* renderer, Core* core)
	{
		std::string fpsStrS = std::to_string(core->GetCurrentFPS()) + "FPS";
		astring fpsStr = ToAString(fpsStrS.c_str());
		float left = ID_LEFT_OFFSET;
		float top = GLOBAL_INFO_TOP;

		renderer->AddText(
			Matrix33().SetIdentity(),
			Matrix33().SetTranslation(left, top),
			Vector2DF(),
			false,
			false,
			Color(255, 255, 255, 255),
			m_font.get(),
			fpsStr.c_str(),
			WritingDirection::Horizontal,
			AlphaBlend::Blend,
			0);
	}

	void ProfilerViewer_Imp::AddDrawCallSprite(Renderer2D* renderer, int drawCallCount)
	{
		std::string countStrS = std::to_string(drawCallCount) + "DrawCall";
		astring countStr = ToAString(countStrS.c_str());
		float left = ID_LEFT_OFFSET + 120;
		float top = GLOBAL_INFO_TOP;

		renderer->AddText(
			Matrix33().SetIdentity(),
			Matrix33().SetTranslation(left, top),
			Vector2DF(),
			false,
			false,
			Color(255, 255, 255, 255),
			m_font.get(),
			countStr.c_str(),
			WritingDirection::Horizontal,
			AlphaBlend::Blend,
			0);
	}

	void ProfilerViewer_Imp::AddIdSprite(Renderer2D* renderer, int index, int id)
	{
		astring idStr = ToAString(std::to_string(id).c_str());
		auto size = m_font->CalcTextureSize(idStr.c_str(), WritingDirection::Horizontal);
		float left = ID_LEFT_OFFSET;
		float top = TOP_OFFSET + (index + 0.5f) * MATER_HEIGHT;

		renderer->AddText(
			Matrix33().SetIdentity(),
			Matrix33().SetTranslation(left, top),
			Vector2DF(size.X, size.Y / 2),
			false,
			false,
			Color(255, 255, 255, 255),
			m_font.get(),
			idStr.c_str(),
			WritingDirection::Horizontal,
			AlphaBlend::Blend,
			0);
	}

	void ProfilerViewer_Imp::AddTimeSprite(Renderer2D* renderer, int index, int time)
	{
		astring timeStr = ToAString(std::to_string(time).c_str());
		auto size = m_font->CalcTextureSize(timeStr.c_str(), WritingDirection::Horizontal);
		float left = ID_LEFT_OFFSET + TIME_LEFT_OFFSET;
		float top = TOP_OFFSET + (index + 0.5f) * MATER_HEIGHT;

		renderer->AddText(
			Matrix33().SetIdentity(),
			Matrix33().SetTranslation(left, top),
			Vector2DF(size.X, size.Y / 2),
			false,
			false,
			Color(255, 255, 255, 255),
			m_font.get(),
			timeStr.c_str(),
			WritingDirection::Horizontal,
			AlphaBlend::Blend,
			0);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void ProfilerViewer_Imp::AddMaterSprite(Renderer2D* renderer, int index, int time)
	{
		Sprite sprite;
		float left = ID_LEFT_OFFSET + TIME_LEFT_OFFSET + MATER_LEFT_OFFSET;
		float top = TOP_OFFSET + index * MATER_HEIGHT;
		float right = left + time / 50.0f;
		float bottom = top + MATER_HEIGHT - 2;

		sprite.positions[0].X = left;
		sprite.positions[0].Y = top;
		sprite.positions[1].X = right;
		sprite.positions[1].Y = top;
		sprite.positions[2].X = right;
		sprite.positions[2].Y = bottom;
		sprite.positions[3].X = left;
		sprite.positions[3].Y = bottom;

		sprite.uvs[0].X = 0;
		sprite.uvs[0].Y = 0;
		sprite.uvs[1].X = 1;
		sprite.uvs[1].Y = 0;
		sprite.uvs[2].X = 1;
		sprite.uvs[2].Y = 1;
		sprite.uvs[3].X = 0;
		sprite.uvs[3].Y = 1;

		renderer->AddSprite(
			sprite.positions,
			sprite.colors,
			sprite.uvs,
			m_materTexture.get(),
			AlphaBlend::Blend,
			0);
	}
}