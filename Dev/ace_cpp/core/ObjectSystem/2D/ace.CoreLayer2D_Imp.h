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
		struct Triangle
		{
			Vector2DF	Pos1, Pos2, Pos3;
			Vector2DF	UV1, UV2, UV3;
			Color		Col1, Col2, Col3;
		};

		std::vector<Triangle>	m_triangles;

		std::list<ObjectPtr> m_objects;
		
		Graphics_Imp*	m_graphics;

		Renderer2D*		m_renderer;
		LayerRenderer*	m_layerRenderer;

		Vector2DI		m_layerSize;
		Vector2DI		m_windowSize;

		RenderTexture_Imp*				m_renderTarget0;
		RenderTexture_Imp*				m_renderTarget1;
		int32_t							m_targetToLayer;

		std::vector<CorePostEffect*>	m_postEffects;

		int m_drawingPriority;

		CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer2D_Imp();

		void CreateRenderTarget(const Vector2DI& size);
	public:
		void SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size );

		void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3);

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

		RenderTexture2D* GetRenderTarget0();
		RenderTexture2D* GetRenderTarget1();
		void SetTargetToLayer(int32_t index);

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