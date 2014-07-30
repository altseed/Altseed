
#pragma once

#include "../../ace.Core.Base.h"

#include "ace.CoreLayer2D.h"
#include "ace.CoreObject2D.h"
#include "ace.CoreCameraObject2D.h"
#include "ace.CoreObject2D_Imp.h"
#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/2D/ace.Renderer2D_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"

namespace ace
{
	class CoreScene;

	class CoreLayer2D_Imp
		: public CoreLayer2D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:
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

		void DrawObjects(Renderer2D* renderer);

	public:
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