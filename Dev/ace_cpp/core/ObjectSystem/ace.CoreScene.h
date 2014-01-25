#pragma once
#include <memory>
#include "ace.CoreLayer.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreScene : public IReference
	{
	protected:
		CoreScene() {}
		virtual ~CoreScene() {}

	public:
		typedef CoreLayer* LayerPtr;
		
		virtual void AddLayer(LayerPtr layer) = 0;
		virtual void RemoveLayer(LayerPtr layer) = 0;

		virtual void Draw() = 0;

		virtual void SetRenderTargetForDrawingLayer() = 0;

		/**
			@brief	ポストエフェクトをかけた結果の画像を取得する。
			@return	ポストエフェクト後の画像
		*/
		virtual RenderTexture2D* GetBaseTarget() = 0;
		
		virtual void BeginPostEffect(CorePostEffect* postEffect) = 0;
		virtual void EndPostEffect(CorePostEffect* postEffect) = 0;
		virtual RenderTexture2D* GetSrcTarget() = 0;
		virtual RenderTexture2D* GetDstTarget() = 0;

		virtual void BeginDrawing() = 0;
		virtual void EndDrawing() = 0;

		virtual void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3) = 0;
	};
}