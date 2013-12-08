
#pragma once

#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreLayer 
		: public IReference
	{
	protected:
		CoreLayer() {}
		virtual ~CoreLayer() {}
	public:
		
		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual bool GetIsDrawn() const = 0;
		virtual void SetIsDrawn(bool value) = 0;

		virtual void AddPostEffect(CorePostEffect* postEffect) = 0;
		virtual void ClearPostEffects() = 0;

		virtual RenderTexture2D* GetFirstRenderTarget() = 0;
		virtual RenderTexture2D* GetRenderTarget0() = 0;
		virtual RenderTexture2D* GetRenderTarget1() = 0;

		/**
		@brief	画面上でのレイヤーの位置を指定する。
		@param	ul	左上座標
		@param	ur	右上座標
		@param	ll	左下座標
		@param	lr	右下座標
		@param	size	描画先の画像の大きさ
		*/
		virtual void SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size) = 0;

		/**
		@brief	レイヤーの画像が描画される三角形を追加する。
		@param	pos1	座標1(画面が-1.0～1.0内に収まるとする)
		@param	uv1		UV座標1
		@param	col1	頂点カラー1
		@param	pos2	座標2(画面が-1.0～1.0内に収まるとする)
		@param	uv2		UV座標2
		@param	col2	頂点カラー2
		@param	pos3	座標3(画面が-1.0～1.0内に収まるとする)
		@param	uv3		UV座標3
		@param	col3	頂点カラー3
		*/
		virtual void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3) = 0;


		/**
		@brief	最終的な描画結果が描かれている描画先を指定する。
		@param	index	インデックス
		*/
		virtual void SetTargetToLayer(int32_t index) = 0;

		virtual void BeginUpdating() = 0;
		virtual void EndUpdating() = 0;

		virtual void BeginDrawing() = 0;
		virtual void Draw() = 0;
		virtual void EndDrawing() = 0;
		virtual void EndDrawingAfterEffects() = 0;
	};
}