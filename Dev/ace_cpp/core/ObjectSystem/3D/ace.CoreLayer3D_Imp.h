
#pragma once

#include "ace.CoreLayer3D.h"

#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/Common/3D/ace.Renderer3D.h"
#include "../../Graphics/Common/2D/ace.LayerRenderer.h"

#include <Math/ace.Vector2DI.h>

namespace ace
{
	class CoreLayer3D_Imp
		: public CoreLayer3D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:

		std::list<ObjectPtr> m_objects;

		Renderer3D*		m_renderer;
		LayerRenderer*	m_layerRenderer;

		CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer3D_Imp();

	public:

		void AddObject(ObjectPtr object3D);
		void RemoveObject(ObjectPtr object3D);

		void Draw();

		// 共通部分
		virtual int GetDrawingPriority() const { return CoreLayer_Imp::GetDrawingPriority(); }
		virtual void SetDrawingPriority(int value) { CoreLayer_Imp::SetDrawingPriority(value); }

		virtual bool GetIsDrawn() const { return CoreLayer_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreLayer_Imp::SetIsDrawn(value); }

		virtual void AddPostEffect(CorePostEffect* postEffect) { AddPostEffect(postEffect); }
		virtual void ClearPostEffects() { ClearPostEffects(); }

		virtual RenderTexture2D* GetRenderTarget0() { return GetRenderTarget0(); }
		virtual RenderTexture2D* GetRenderTarget1() { return GetRenderTarget1(); }
		virtual void SetTargetToLayer(int32_t index) { SetTargetToLayer(index); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}