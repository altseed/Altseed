#pragma once
#include "ace.CoreLayer2D.h"
#include "ace.ICoreObject2D.h"
#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "../../Graphics/Common/2D/ace.LayerRenderer.h"
#include <list>
#include <memory>

namespace ace
{
	class CoreLayer2D_Imp
		: public CoreLayer2D
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:
		std::list<ObjectPtr> m_objects;
		
		Graphics_Imp*	m_graphics;

		Renderer2D*		m_renderer;
		LayerRenderer*	m_layerRenderer;

		Vector2DI		m_layerSize;

		RenderTexture_Imp*				m_renderTargetDefaultToPostEffect;
		RenderTexture_Imp*				m_renderTargetPostEffectToLayer;

		std::vector<CorePostEffect*>	m_postEffects;

		int m_drawingPriority;

		CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer2D_Imp();
	public:
		void AddObject(ObjectPtr object);
		void RemoveObject(ObjectPtr object);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		void BeginDrawing();
		void Draw();
		void EndDrawing();
		void EndDrawingAfterEffects();

		void AddPostEffect(CorePostEffect* postEffect);

		void ClearPostEffects();

		RenderTexture2D* GetRenderTargetDefaultToPostEffect();
		RenderTexture2D* GetRenderTargetPostEffectToLayer();

#if !SWIG
		Renderer2D* GetRenderer() const;
#endif

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}