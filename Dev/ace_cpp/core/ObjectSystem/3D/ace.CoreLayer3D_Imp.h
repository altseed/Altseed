
#pragma once

#include "ace.CoreLayer3D.h"

#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/Common/3D/ace.Renderer3D.h"
#include "../../Graphics/Common/2D/ace.LayerRenderer.h"

#include <Math/ace.Vector2DI.h>

namespace ace
{
	class CoreScene;

	class CoreLayer3D_Imp
		: public CoreLayer3D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:

		std::set<ObjectPtr> m_objects;

		Renderer3D*		m_renderer;

		bool			m_isDrawnTemp;

		CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer3D_Imp();

	public:

		void AddObject(ObjectPtr object3D);
		void RemoveObject(ObjectPtr object3D);

		void BeginUpdating();
		void EndUpdating();

		void BeginDrawing();
		
		void Draw();

		void EndDrawing();

		Renderer3D* GetRenderer() { return m_renderer; }

		// 共通部分
		virtual int GetDrawingPriority() const { return CoreLayer_Imp::GetDrawingPriority(); }
		virtual void SetDrawingPriority(int value) { CoreLayer_Imp::SetDrawingPriority(value); }

		virtual bool GetIsDrawn() const { return CoreLayer_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreLayer_Imp::SetIsDrawn(value); }

		virtual void AddPostEffect(CorePostEffect* postEffect) { CoreLayer_Imp::AddPostEffect(postEffect); }
		virtual void ClearPostEffects() { CoreLayer_Imp::ClearPostEffects(); }
#if !SWIG
	public:
		virtual void SetScene(CoreScene* scene) { CoreLayer_Imp::SetScene(scene); }

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}