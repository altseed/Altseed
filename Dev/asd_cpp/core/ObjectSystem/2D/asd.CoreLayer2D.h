#pragma once
#include <list>
#include "asd.CoreObject2D.h"
#include "../../Graphics/2D/asd.Renderer2D_Imp.h"
#include "../asd.CoreLayer.h"
#include "../../Shape/asd.CoreShape.h"

namespace asd
{
	class CoreLayer2D
		: public CoreLayer
	{
	public:
		typedef CoreObject2D* ObjectPtr;

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

		virtual void PrepareToAddObject(ObjectPtr object2D) = 0;
		virtual void PrepareToRemoveObject(ObjectPtr object2D) = 0;
		virtual void AddObject(ObjectPtr object2D) = 0;
		virtual void RemoveObject(ObjectPtr object2D) = 0;
		virtual void Clear() = 0;

		virtual bool GetIsDistortionEnabled() const = 0;
		virtual void SetIsDistortionEnabled(bool value) = 0;

		virtual void DrawSpriteAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawSpriteWithMaterialAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			Material2D* material, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawTextAdditionally(Vector2DF pos, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawRectangleAdditionally(const RectF& drawingArea, const Color& color, const RectF& uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;
		virtual void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;
		virtual void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;
		virtual void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, int startingCorner, int endingCorner, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority) = 0;

		virtual void DrawShapeAdditionally(CoreShape* shape, Color color, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) = 0;
#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
		virtual Graphics* GetGraphicsImp() = 0;
#endif
	};
}