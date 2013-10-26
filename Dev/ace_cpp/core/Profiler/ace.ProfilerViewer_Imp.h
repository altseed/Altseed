#pragma once

#include <memory>
#include "ace.ProfilerViewer.h"
#include "../Window/ace.Window.h"
#include "../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "../Log/ace.Log.h"

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
		Renderer2D* m_renderer;
		std::shared_ptr<Texture2D> m_materTexture;

		Sprite* CreatePolygonOfMater(int index, int time);

	public:
		ProfilerViewer_Imp(Graphics_Imp* graphics, Log* log, Vector2DI windowSize);
		virtual ~ProfilerViewer_Imp();

		static ProfilerViewer_Imp* Create(Profiler_Imp* profiler,
			Graphics_Imp* graphics,
			Log* logger,
			Vector2DI windowSize);

		void SetProfiler(Profiler_Imp* profiler);
		void Draw();
	};
}