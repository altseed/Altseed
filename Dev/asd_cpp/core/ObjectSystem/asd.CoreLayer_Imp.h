
#pragma once

#include <Math/asd.Vector2DI.h>
#include <Math/asd.Vector2DF.h>

#include "asd.CoreLayer.h"

namespace asd
{
	class CoreLayer_Imp
		: public CoreLayer
	{
	protected:

		struct Triangle
		{
			Vector2DF	Pos1, Pos2, Pos3;
			Vector2DF	UV1, UV2, UV3;
			Color		Col1, Col2, Col3;
		};

		std::vector<Triangle>	m_triangles;


		Graphics_Imp*	m_graphics;

		Vector2DI		m_layerSize;
		Vector2DI		m_windowSize;

		bool			m_isDrawn;
		int32_t			m_drawingPriority;

		LayerRenderer*	m_layerRenderer;

		std::vector<CorePostEffect*>	m_postEffects;

		CoreScene* m_scene;

		int64_t m_previousUpdateTime;
		int64_t m_timeAtUpdateStart;

		CoreLayer_Imp(Graphics* graphics, Vector2DI windowSize);
		virtual ~CoreLayer_Imp();
	public:
		virtual int GetDrawingPriority() const;
		virtual void SetDrawingPriority(int value);

		virtual bool GetIsDrawn() const;
		virtual void SetIsDrawn(bool value);

		virtual void AddPostEffect(CorePostEffect* postEffect);
		virtual void ClearPostEffects();

		void BeginMeasureUpdateTime();
		void EndMeasureUpdateTime();
		int GetTimeForUpdate() const;

#if !SWIG
		virtual void SetScene(CoreScene* scene);
#endif
	};
}