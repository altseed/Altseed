#pragma once
#include <list>
#include <memory>
#include "asd.CoreScene.h"

#include <Math/asd.Vector2DI.h>
#include <Math/asd.Vector2DF.h>

namespace asd
{
	class CoreScene_Imp
		: public CoreScene
		, public ReferenceObject
	{
		friend class ObjectSystemFactory_Imp;
	private:

		PostEffectRenderer*		m_postEffectRenderer;

		std::list<LayerPtr> m_layers;

		Vector2DI			m_windowSize;
		bool				m_hdrMode;

		Graphics_Imp*		m_graphics;
		RenderTexture2D_Imp*	m_baseTarget0;
		RenderTexture2D_Imp*	m_baseTarget1;
		int32_t				m_targetIndex;

		/**
			@brief	レンダーターゲットを必要ならば更新する。
		*/
		void RenewRenderTarget(Vector2DI windowSize, bool isHDRMode);

		CoreScene_Imp(Graphics* graphics, Vector2DI windowSize);
		virtual ~CoreScene_Imp();
	public:

		bool GetHDRMode() const override;
		void SetHDRMode(bool value) override;

		void AddLayer(LayerPtr layer);
		void RemoveLayer(LayerPtr layer);

		void SetRenderTargetForDrawingLayer();

		RenderTexture2D* GetBaseTarget();
		void BeginPostEffect(CorePostEffect* postEffect);
		void EndPostEffect(CorePostEffect* postEffect);
		RenderTexture2D* GetSrcTarget();
		RenderTexture2D* GetDstTarget();

		void BeginDrawing();

		void EndDrawing();

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}