
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>

#include "ace.CoreLayer.h"

namespace ace
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

		RenderTexture_Imp*				m_renderTarget0;
		RenderTexture_Imp*				m_renderTarget1;
		int32_t							m_targetToLayer;

		std::vector<CorePostEffect*>	m_postEffects;

		CoreLayer_Imp(Graphics* graphics, Vector2DI windowSize);
		virtual ~CoreLayer_Imp();
	public:

		virtual int GetDrawingPriority() const;
		virtual void SetDrawingPriority(int value);

		virtual bool GetIsDrawn() const;
		virtual void SetIsDrawn(bool value);

		virtual void AddPostEffect(CorePostEffect* postEffect);
		virtual void ClearPostEffects();

		virtual RenderTexture2D* GetRenderTarget0();
		virtual RenderTexture2D* GetRenderTarget1();
		virtual void SetTargetToLayer(int32_t index);

		void CreateRenderTarget(const Vector2DI& size);

	};
}