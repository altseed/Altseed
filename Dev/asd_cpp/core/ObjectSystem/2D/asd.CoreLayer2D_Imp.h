
#pragma once

#include "../../asd.Core.Base.h"

#include "asd.CoreLayer2D.h"
#include "asd.CoreObject2D.h"
#include "asd.CoreCameraObject2D.h"
#include "../asd.CoreLayer_Imp.h"
#include "../../Graphics/2D/asd.Renderer2D_Imp.h"
#include "../../Graphics/asd.Graphics_Imp.h"
#include "asd.Culling2D.h"
#include <queue>

namespace asd
{
	class CoreScene;
	class CoreObject2D_Imp;
	class CoreChip2D_Imp;

	class CoreLayer2D_Imp
		: public CoreLayer2D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:
		struct Sprite
		{
			std::array<Vector2DF, 4> pos;
			std::array<Color, 4> col;
			std::array<Vector2DF, 4> uv;
			std::shared_ptr<Texture2D> Texture_;
			std::shared_ptr<Material2D> Material_;
			AlphaBlendMode AlphaBlend_;
			int32_t Priority;
		};

		struct Text
		{
			Vector2DF				Position_;
			Color					Color_;
			std::shared_ptr<Font>	Font_;
			astring					Text_;
			WritingDirection		WritingDirection_;
			AlphaBlendMode				AlphaBlend_;
			int32_t					Priority_;
		};

		std::vector<Sprite>		sprites;
		std::vector<Text>		texts;

		std::list <CoreCameraObject2D*> m_cameras;
		std::list<ObjectPtr> m_objects;

		Renderer2D_Imp*		m_renderer;
		Renderer2D_Imp*		m_rendererForCamera;
		Core*				core = nullptr;

		CoreLayer2D_Imp(Core* core, Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer2D_Imp();

		/**
			@brief	Impに型変換する。
			@note
			CoreObject2D_ImpがCoreObject2Dを継承していない、かつ、dynamic_castを使用しないため
		*/
		CoreObject2D_Imp* CoreObject2DToImp(ObjectPtr obj);

		void DrawAdditionalObjects(int32_t group);
		void ClearAdditionalObjects();

#if __CULLING_2D__
		void DrawObjectsWithCulling(int32_t group, RectF drawRange);
#else
		void DrawObjectsWithoutCulling();
#endif

#if __CULLING_2D__
		culling2d::World *world = nullptr;
		std::deque<culling2d::Object*> transformedObjects;
#endif
	public:
#if __CULLING_2D__

		void AddTransformedObject(culling2d::Object* object);
		void RemoveTransformedObject(culling2d::Object* object);

		void AddChipCullingObject(CoreChip2D_Imp *chip, uint32_t firstSortKey);
		void RemoveChipCullingObject(CoreChip2D_Imp *chip);
#endif
		void SetSize(Vector2DI size) override;

		void AddObject(ObjectPtr object);
		void RemoveObject(ObjectPtr object);
		void Clear();

		void BeginUpdating(bool isUpdated) override;
		void EndUpdating(bool isUpdated) override;

		void BeginDrawing();
		void Draw();
		void EndDrawing();

		// 共通部分
		virtual int GetDrawingPriority() const { return CoreLayer_Imp::GetDrawingPriority(); }
		virtual void SetDrawingPriority(int value) { CoreLayer_Imp::SetDrawingPriority(value); }

		virtual bool GetIsDrawn() const { return CoreLayer_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreLayer_Imp::SetIsDrawn(value); }

		virtual void AddPostEffect(CorePostEffect* postEffect) { CoreLayer_Imp::AddPostEffect(postEffect); }
		virtual void ClearPostEffects() { CoreLayer_Imp::ClearPostEffects(); }

		void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawSpriteWithMaterialAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Material2D* material, AlphaBlendMode alphaBlend, int32_t priority) override;

		void DrawTextAdditionally(Vector2DF pos, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority) override;

		void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int numberOfCorners, int startingCorner, int endingCorner, float angle, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority) override;
		void DrawShapeAdditionally(CoreShape* shape, Color color, Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority) override;

		void BeginMeasureUpdateTime() { CoreLayer_Imp::BeginMeasureUpdateTime(); }
		void EndMeasureUpdateTime() { CoreLayer_Imp::EndMeasureUpdateTime(); }
		int GetTimeForUpdate() const { return CoreLayer_Imp::GetTimeForUpdate(); }

#if !SWIG
	public:
		Renderer2D* GetRenderer() const { return m_renderer; }
		Graphics* GetGraphicsImp() { return m_graphics; }

		virtual void SetScene(CoreScene* scene) { CoreLayer_Imp::SetScene(scene); }

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}