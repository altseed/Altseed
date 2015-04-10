#pragma once

#include <memory>
#include "ace.ProfilerViewer.h"
#include "../Window/ace.Window.h"
#include "../Graphics/2D/ace.Renderer2D_Imp.h"
#include "../Log/ace.Log.h"
#include "../Core/ace.Core.h"

namespace ace
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
		void DrawProfiledInfoSection(int top);
		void DrawSprite(RectF dst, Color color, int priority);
		void DrawTextSprite(Vector2DF position, Color color, std::string text);

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

		void SetProfiler(Profiler_Imp* profiler);
		void Draw();
	};
}