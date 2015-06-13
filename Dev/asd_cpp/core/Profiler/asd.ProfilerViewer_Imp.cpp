#include <cmath>
#include "asd.ProfilerViewer_Imp.h"

#include "../Graphics/asd.Graphics_Imp.h"

#include "../Graphics/Resource/asd.Texture2D_Imp.h"
#include "../Graphics/Resource/asd.Font_Imp.h"
#include "../Graphics/Resource/asd.Texture2D_Imp.h"

#include "../Graphics/2D/asd.Renderer2D_Imp.h"
#include "../Log/asd.Log_Imp.h"

#include "asd.InternalFont.h"
#include "asd.InternalFontTex0.h"
#include "asd.InternalFontTex1.h"
#include "asd.InternalFontTex2.h"

using namespace std;

namespace asd
{
	const int SECTION_SPAN = 5;
	const int COLUMN_HEADER_OFFSET = 5;
	const int ROW_HEIGHT = 16;
	const int COLUMN1_OFFSET = 15;
	const int COLUMN2_OFFSET = 80;
	const int COLUMN3_OFFSET = 160;
	const int DRAWCALL_OFFSET = 80;
	const Color SECTION_HEADER_COLOR = Color(255, 255, 0, 255);
	const Color HEADER_COLOR = Color(128, 128, 128, 255);
	const Color CONTENT_COLOR = Color(255, 255, 255, 255);

	std::shared_ptr<Font> ProfilerViewer_Imp::CreateFont_()
	{
		auto tex0 = (Texture2D*)m_graphics->CreateTexture2D_Imp_Internal(m_graphics, (uint8_t*)g_internalFontTex0, sizeof(g_internalFontTex0));
		auto tex1 = (Texture2D*)m_graphics->CreateTexture2D_Imp_Internal(m_graphics, (uint8_t*)g_internalFontTex1, sizeof(g_internalFontTex1));
		auto tex2 = (Texture2D*) m_graphics->CreateTexture2D_Imp_Internal(m_graphics, (uint8_t*) g_internalFontTex2, sizeof(g_internalFontTex2));

		std::vector<std::shared_ptr<Texture2D>> textures;
		textures.push_back(CreateSharedPtrWithReleaseDLL(tex0));
		textures.push_back(CreateSharedPtrWithReleaseDLL(tex1));
		textures.push_back(CreateSharedPtrWithReleaseDLL(tex2));

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
		LayerProfiler_Imp* layerProfiler,
		Core* core,
		Vector2DI windowSize)
		: m_profiler(profiler)
		, m_layerProfiler(layerProfiler)
		, m_renderer(nullptr)
		, m_windowSize(windowSize)
		, m_graphics(graphics)
		, m_core(core)
		, m_materTexture(nullptr)
		, log(log)
	{
		SafeAddRef(m_profiler);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp::~ProfilerViewer_Imp()
	{
		SafeDelete(m_renderer);
		SafeRelease(m_profiler);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp* ProfilerViewer_Imp::Create(
		Core* core,
		Profiler_Imp* profiler,
		LayerProfiler_Imp* layerProfiler,
		Graphics_Imp* graphics,
		Log* logger,
		Vector2DI windowSize)
	{
		logger->WriteHeading("プロファイラビュアー");

		auto temp = new ProfilerViewer_Imp(graphics, logger, profiler, layerProfiler, core, windowSize);

		logger->WriteLine("初期化成功");

		return temp;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void ProfilerViewer_Imp::Draw()
	{
		GenerateRenderer();

		auto top = DrawGlobalInfoSection();
		top = DrawLayerInfoSection(top);
		DrawProfiledInfoSection(top);

		m_renderer->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
		m_renderer->DrawCache();
		m_renderer->ClearCache();
	}

	int ProfilerViewer_Imp::DrawGlobalInfoSection()
	{
		DrawSprite(RectF(0, 0, m_windowSize.X, SECTION_SPAN * 2 + ROW_HEIGHT * 2), Color(64, 64, 0, 128), 0);

		DrawTextSprite(
			Vector2DF(COLUMN_HEADER_OFFSET, SECTION_SPAN),
			SECTION_HEADER_COLOR,
			ToAString("Global Information"));

		auto fpsStr = to_string((int)m_core->GetCurrentFPS()) + "fps";
		DrawTextSprite(
			Vector2DF(COLUMN1_OFFSET, SECTION_SPAN + ROW_HEIGHT),
			CONTENT_COLOR,
			ToAString(fpsStr.c_str()));

		auto drawCallStr = to_string(m_graphics->GetDrawCallCount()) + "DrawCall";
		DrawTextSprite(
			Vector2DF(DRAWCALL_OFFSET, SECTION_SPAN + ROW_HEIGHT),
			CONTENT_COLOR,
			ToAString(drawCallStr.c_str()));

		return SECTION_SPAN * 2 + ROW_HEIGHT * 2;
	}

	int ProfilerViewer_Imp::DrawLayerInfoSection(int top)
	{
		DrawTextSprite(
			Vector2DF(COLUMN_HEADER_OFFSET, top),
			SECTION_HEADER_COLOR,
			ToAString("Layer Information"));

		DrawTextSprite(
			Vector2DF(COLUMN1_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("Name"));

		DrawTextSprite(
			Vector2DF(COLUMN2_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("Time(ns)"));

		DrawTextSprite(
			Vector2DF(COLUMN3_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("Objects"));

		auto bodyTop = top + ROW_HEIGHT * 2;
		int index = 0;
		for (auto& profile : m_layerProfiler->GetProfiles())
		{
			DrawTextSprite(
				Vector2DF(COLUMN1_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				profile->GetName());

			DrawTextSprite(
				Vector2DF(COLUMN2_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				ToAString(to_string(profile->GetTime()).c_str()));

			DrawTextSprite(
				Vector2DF(COLUMN3_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				ToAString(to_string(profile->GetObjectCount()).c_str()));

			++index;
		}

		DrawSprite(RectF(0, top, m_windowSize.X, (index + 2)*ROW_HEIGHT + SECTION_SPAN), Color(64, 0, 64, 128), 0);

		return top + (index + 2) * ROW_HEIGHT + SECTION_SPAN;
	}

	void ProfilerViewer_Imp::DrawProfiledInfoSection(int top)
	{
		DrawTextSprite(
			Vector2DF(COLUMN_HEADER_OFFSET, top),
			SECTION_HEADER_COLOR,
			ToAString("Profiled Information"));

		DrawTextSprite(
			Vector2DF(COLUMN1_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("ID"));

		DrawTextSprite(
			Vector2DF(COLUMN2_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("Time(ns)"));

		DrawTextSprite(
			Vector2DF(COLUMN3_OFFSET, top + ROW_HEIGHT),
			HEADER_COLOR,
			ToAString("Processor"));

		int bodyTop = top + ROW_HEIGHT * 2;
		int index = 0;
		for (auto& profile : m_profiler->GetProfiles())
		{
			auto perf = profile->GetLastLog();
			if (perf == nullptr)
			{
				continue;
			}

			int time = perf->GetEndTime() - perf->GetStartTime();

			DrawTextSprite(
				Vector2DF(COLUMN1_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				ToAString(to_string(profile->GetID()).c_str()));

			DrawTextSprite(
				Vector2DF(COLUMN2_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				ToAString(to_string(time).c_str()));

			DrawTextSprite(
				Vector2DF(COLUMN3_OFFSET, bodyTop + index * ROW_HEIGHT),
				CONTENT_COLOR,
				ToAString(to_string(perf->GetProcessorNumber()).c_str()));

			++index;
		}

		DrawSprite(RectF(0, top, m_windowSize.X, (index + 2)*ROW_HEIGHT + SECTION_SPAN), Color(0, 64, 64, 128), 0);
	}

	void ProfilerViewer_Imp::DrawSprite(RectF dst, Color color, int priority)
	{
		Sprite sprite;
		float left = dst.X;
		float top = dst.Y;
		float right = left + dst.Width;
		float bottom = top + dst.Height;

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

		sprite.colors[0] = color;
		sprite.colors[1] = color;
		sprite.colors[2] = color;
		sprite.colors[3] = color;

		m_renderer->AddSprite(
			sprite.positions,
			sprite.colors,
			sprite.uvs,
			m_materTexture.get(),
			AlphaBlendMode::Blend,
			priority);
	}

	void ProfilerViewer_Imp::DrawTextSprite(Vector2DF position, Color color, astring text)
	{
		m_renderer->AddText(
			Matrix33().SetIdentity(),
			Matrix33().SetTranslation(position.X, position.Y),
			Vector2DF(),
			false,
			false,
			color,
			m_font.get(),
			text.c_str(),
			WritingDirection::Horizontal,
			AlphaBlendMode::Blend,
			1);
	}

}