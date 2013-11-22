#pragma once
#include "ace.CoreLayer2D.h"
#include "ace.CoreObject2D.h"
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
		struct Triangle
		{
			Vector2DF	Pos1, Pos2, Pos3;
			Vector2DF	UV1, UV2, UV3;
			Color		Col1, Col2, Col3;
		};

		std::vector<Triangle>	m_triangles;

		std::list<ObjectPtr> m_objects;

		Renderer2D*		m_renderer;

		CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize);
		virtual ~CoreLayer2D_Imp();

	public:
		void SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size );

		void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3);


		void AddObject(ObjectPtr object);
		void RemoveObject(ObjectPtr object);

		void BeginDrawing();
		void Draw();
		void EndDrawing();
		void EndDrawingAfterEffects();



		// 共通部分
		virtual int GetDrawingPriority() const { return CoreLayer_Imp::GetDrawingPriority(); }
		virtual void SetDrawingPriority(int value) { CoreLayer_Imp::SetDrawingPriority(value); }

		virtual bool GetIsDrawn() const { return CoreLayer_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreLayer_Imp::SetIsDrawn(value); }

		virtual void AddPostEffect(CorePostEffect* postEffect) { CoreLayer_Imp::AddPostEffect(postEffect); }
		virtual void ClearPostEffects() { CoreLayer_Imp::ClearPostEffects(); }

		virtual RenderTexture2D* GetRenderTarget0() { return CoreLayer_Imp::GetRenderTarget0(); }
		virtual RenderTexture2D* GetRenderTarget1() { return CoreLayer_Imp::GetRenderTarget1(); }
		virtual void SetTargetToLayer(int32_t index) { CoreLayer_Imp::SetTargetToLayer(index); }

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