﻿#pragma once

#include <memory>
#include "asd.ProfilerViewer.h"
#include "../Window/asd.Window.h"
#include "../Graphics/2D/asd.Renderer2D_Imp.h"
#include "../Log/asd.Log.h"
#include "../Core/asd.Core.h"
#include "asd.LayerProfiler.h"
#include "asd.LayerProfiler_Imp.h"

namespace asd
{
	class ProfilerViewer_Imp : public ProfilerViewer
	{
	private:
		typedef struct TSprite
		{
			Vector2DF positions[4];
			Color colors[4];
			Vector2DF uvs[4];
		} Sprite;

		Profiler_Imp* m_profiler;
		LayerProfiler_Imp* m_layerProfiler;
		
		Graphics_Imp* m_graphics;
		Core* m_core;
		Vector2DI	m_windowSize;
		std::shared_ptr<Texture2D> m_materTexture;
		std::shared_ptr<Font> m_font;

		Renderer2D* m_renderer = nullptr;
		Log*	log = nullptr;
		bool	isRendererGenerated = false;

		int DrawGlobalInfoSection();
		int DrawLayerInfoSection(int top);
		void DrawProfiledInfoSection(int top);
		void DrawSprite(RectF dst, Color color, int priority);
		void DrawTextSprite(Vector2DF position, Color color, astring text);

		std::shared_ptr<Font> CreateFont_();

		void GenerateRenderer();

	public:
		ProfilerViewer_Imp(
			Graphics_Imp* graphics,
			Log* log,
			Profiler_Imp* profiler,
			LayerProfiler_Imp* layerProfiler,
			Core* core,
			Vector2DI windowSize);
		virtual ~ProfilerViewer_Imp();

		static ProfilerViewer_Imp* Create(
			Core* core,
			Profiler_Imp* profiler,
			LayerProfiler_Imp* layerProfiler,
			Graphics_Imp* graphics,
			Log* logger,
			Vector2DI windowSize);

		void Draw();
	};
}