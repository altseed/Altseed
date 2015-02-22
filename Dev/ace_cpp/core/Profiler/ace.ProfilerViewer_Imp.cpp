#include "ace.ProfilerViewer_Imp.h"
#include "../Graphics/ace.Graphics_Imp.h"
#include "../Graphics/Resource/ace.Texture2D_Imp.h"
#include "../Graphics/2D/ace.Renderer2D_Imp.h"
#include "../Log/ace.Log_Imp.h"
#include <cmath>

namespace ace
{
	const int MATER_HEIGHT = 32;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp::ProfilerViewer_Imp(Graphics_Imp* graphics, Renderer2D* renderer, Log* log, Profiler_Imp* profiler, Vector2DI windowSize)
		: m_profiler(profiler)
		, m_renderer(renderer)
		, m_materTexture(nullptr)
		, m_windowSize(windowSize)
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
		Profiler_Imp* profiler,
		Graphics_Imp* graphics,
		Log* logger,
		Vector2DI windowSize)
	{
		logger->WriteHeading("プロファイラビュアー");

		auto renderer = new Renderer2D_Imp(graphics, logger);
		auto temp = new ProfilerViewer_Imp(graphics, renderer, logger, profiler, windowSize);
		
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
		int index = 0;
		for (auto& profile : m_profiler->GetProfiles())
		{
			auto perf = profile->GetLastLog();
			if (perf == nullptr)
			{
				continue;
			}

			int time = perf->GetEndTime() - perf->GetStartTime();

			auto p = CreatePolygonOfMater(index, time);
			m_renderer->AddSprite(
				p->positions,
				p->colors,
				p->uvs,
				m_materTexture.get(),
				AlphaBlend::Blend,
				0);

			++index;
		}

		m_renderer->SetArea(RectF(0, 0, m_windowSize.X, m_windowSize.Y), 0.0f);
		m_renderer->DrawCache();
		m_renderer->ClearCache();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ProfilerViewer_Imp::Sprite* ProfilerViewer_Imp::CreatePolygonOfMater(int index, int time)
	{
		Sprite* sprite = new Sprite();
		float left = 20;
		float top = 20 + index * MATER_HEIGHT;
		float right = 20 + time / 100.0f;
		float bottom = 20 + (index + 1) * (MATER_HEIGHT - 2);

		sprite->positions[0].X = left;
		sprite->positions[0].Y = top;
		sprite->positions[1].X = right;
		sprite->positions[1].Y = top;
		sprite->positions[2].X = right;
		sprite->positions[2].Y = bottom;
		sprite->positions[3].X = left;
		sprite->positions[3].Y = bottom;

		sprite->uvs[0].X = 0;
		sprite->uvs[0].Y = 0;
		sprite->uvs[1].X = 1;
		sprite->uvs[1].Y = 0;
		sprite->uvs[2].X = 1;
		sprite->uvs[2].Y = 1;
		sprite->uvs[3].X = 0;
		sprite->uvs[3].Y = 1;

		return sprite;
	}
}