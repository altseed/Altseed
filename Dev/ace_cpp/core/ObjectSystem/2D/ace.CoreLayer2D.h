#pragma once
#include <list>
#include "ace.ICoreObject2D.h"
#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "../ace.CoreLayer.h"

namespace ace
{
	class CoreLayer2D : public CoreLayer
	{
	public:
		typedef ICoreObject2D* ObjectPtr;

		virtual void AddObject(ObjectPtr object2D) = 0;
		virtual void RemoveObject(ObjectPtr object2D) = 0;

		virtual void BeginDrawing() = 0;
		virtual void Draw() = 0;
		virtual void EndDrawing() = 0;
		virtual void EndDrawingAfterEffects() = 0;

		virtual void AddPostEffect(CorePostEffect* postEffect) = 0;

		virtual void ClearPostEffects() = 0;

		virtual RenderTexture2D* GetRenderTarget0() = 0;
		virtual RenderTexture2D* GetRenderTarget1() = 0;
		
		/**
			@brief	最終的な描画結果が描かれている描画先を指定する。
			@param	index	インデックス
		*/
		virtual void SetTargetToLayer(int32_t index) = 0;

#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
#endif
	};
}