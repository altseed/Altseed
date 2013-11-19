#pragma once
#include <list>
#include "ace.CoreObject2D.h"
#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "../ace.CoreLayer.h"

namespace ace
{
	class CoreLayer2D 
		: public CoreLayer
	{
	public:

		/**
			@brief	コンストラクタ
			@note
			中身がなくともないと多重継承に失敗する。
		*/
		CoreLayer2D(){}

		/**
		@brief	デストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		virtual ~CoreLayer2D(){}

		typedef CoreObject2D* ObjectPtr;

		virtual void AddObject(ObjectPtr object2D) = 0;
		virtual void RemoveObject(ObjectPtr object2D) = 0;

		virtual void BeginDrawing() = 0;
		virtual void Draw() = 0;
		virtual void EndDrawing() = 0;
		virtual void EndDrawingAfterEffects() = 0;

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

#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
#endif
	};
}