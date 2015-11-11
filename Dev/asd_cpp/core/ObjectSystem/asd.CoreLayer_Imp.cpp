
#include "asd.CoreLayer_Imp.h"

#include "PostEffect/asd.CorePostEffect_Imp.h"

#include "../Graphics/asd.Graphics_Imp.h"
#include "../Graphics/2D/asd.LayerRenderer.h"
#include <Utility/asd.Timer.h>

#include <iostream>
#include <fstream>
#include <string>

namespace asd
{
	static bool isInitialized = false;
	static std::ofstream output;

	void CoreLayer_Imp::WriteLog(const char* text)
	{
		// デバッグ用
		/*
		if (!isInitialized)
		{
			output = std::ofstream("debug.txt");
			isInitialized = true;
		}

		output << text << std::endl;
		output.flush();
		*/
	}

	CoreLayer_Imp::CoreLayer_Imp(Graphics* graphics, Vector2DI windowSize)
		: m_graphics(nullptr)
		, m_isDrawn(true)
		, m_drawingPriority(0)
		, m_layerSize(windowSize)
		, m_windowSize(windowSize)
		, m_layerRenderer(nullptr)
		, m_previousUpdateTime(0)
		, m_timeAtUpdateStart(0)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		WriteLog("Start initializing m_layerRenderer");
		m_layerRenderer = new LayerRenderer(graphics);
		m_layerRenderer->SetWindowSize(windowSize);
		WriteLog("End initializing m_layerRenderer");

		WriteLog("Start initializing m_layerRenderer2");
		{
			asd::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = windowSize.X;
			lpos[2].Y = windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}

		WriteLog("End initializing m_layerRenderer2");
	}

	CoreLayer_Imp::~CoreLayer_Imp()
	{
		ClearPostEffects();

		SafeRelease(m_layerRenderer);

		SafeRelease(m_graphics);
	}

	int CoreLayer_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreLayer_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

	bool CoreLayer_Imp::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void CoreLayer_Imp::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

	void CoreLayer_Imp::AddPostEffect(CorePostEffect* postEffect)
	{
		SafeAddRef(postEffect);
		m_postEffects.push_back(postEffect);
	}

	void CoreLayer_Imp::ClearPostEffects()
	{
		for (auto& p : m_postEffects)
		{
			SafeRelease(p);
		}

		m_postEffects.clear();
	}

	void CoreLayer_Imp::SetScene(CoreScene* scene)
	{
		m_scene = scene;
	}

	void CoreLayer_Imp::BeginMeasureUpdateTime()
	{
		m_timeAtUpdateStart = asd::GetTime();
	}

	void CoreLayer_Imp::EndMeasureUpdateTime()
	{
		m_previousUpdateTime = asd::GetTime() - m_timeAtUpdateStart;
	}

	int CoreLayer_Imp::GetTimeForUpdate() const
	{
		return m_previousUpdateTime;
	}

}