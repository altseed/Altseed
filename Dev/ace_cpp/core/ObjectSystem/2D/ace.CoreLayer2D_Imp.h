#pragma once
#include "ace.CoreLayer2D.h"
#include "ace.CoreObject2D.h"
#include "ace.CoreCameraObject2D.h"
#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include <list>
#include <memory>

namespace ace
{
	class CoreLayer2D_Imp
		: public CoreLayer2D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:
		std::set<CoreCameraObject2D*> m_cameras;
		std::list<ObjectPtr> m_objects;

		Renderer2D*		m_renderer;

		CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer2D_Imp();

		void DrawObjects(CoreCameraObject2D* camera);

	public:
		void AddObject(ObjectPtr object);
		void RemoveObject(ObjectPtr object);

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
#if !SWIG
		Renderer2D* GetRenderer() const { return m_renderer; }
#endif

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}