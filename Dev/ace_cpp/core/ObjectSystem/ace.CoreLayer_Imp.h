
#pragma once

#include <Math/ace.Vector2DI.h>
#include "ace.CoreLayer.h"

namespace ace
{
	class CoreLayer_Imp 
		: public CoreLayer
	{
	protected:

		Graphics_Imp*	m_graphics;

		Vector2DI		m_layerSize;
		Vector2DI		m_windowSize;

		bool			m_isDrawn;
		int32_t			m_drawingPriority;

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

		virtual void AddPostEffect(CorePostEffect* postEffect) override;
		virtual void ClearPostEffects() override;

		virtual RenderTexture2D* GetRenderTarget0() override;
		virtual RenderTexture2D* GetRenderTarget1() override;
		virtual void SetTargetToLayer(int32_t index) override;

		void CreateRenderTarget(const Vector2DI& size);

	};
}