
#pragma once

#include "../../ace.Core.Base.h"

#include "ace.CoreLayer2D.h"
#include "ace.CoreObject2D.h"
#include "ace.CoreCameraObject2D.h"
#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/2D/ace.Renderer2D_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"
#include "ace.Culling2D.h"
#include <queue>

namespace ace
{
	class CoreScene;
	class CoreObject2D_Imp;
	class Chip2D_Imp;

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
			AlphaBlend AlphaBlend_;
			int32_t Priority;
		};

		struct Text
		{
			Vector2DF				Position_;
			Color					Color_;
			std::shared_ptr<Font>	Font_;
			astring					Text_;
			WritingDirection		WritingDirection_;
			AlphaBlend				AlphaBlend_;
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

		void DrawAdditionalObjects();
		void ClearAdditionalObjects();

#if __CULLING_2D__
		void DrawObjectsWithCulling(RectF drawRange);
#else
		void DrawObjectsWithoutCulling();
#endif

#if __CULLING_2D__
		culling2d::World *world = nullptr;
#endif
	public:
#if __CULLING_2D__
		culling2d::World *GetCullingWorld() const;
		std::deque<culling2d::Object*> TransformedObjects;

		void AddChipCullingObject(Chip2D_Imp *chip, uint32_t firstSortKey);
		void RemoveChipCullingObject(Chip2D_Imp *chip);
#endif
		void AddObject(ObjectPtr object);
		void RemoveObject(ObjectPtr object);
		void Clear();

		void BeginUpdating();
		void EndUpdating();

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
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority);

		void DrawTextAdditionally(Vector2DF pos, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlend alphaBlend, int32_t priority) override;

		void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) override;
		void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) override;
		void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) override;
		void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) override;
		void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) override;
		void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, Color color, float thickness, AlphaBlend alphaBlend, int32_t priority) override;

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