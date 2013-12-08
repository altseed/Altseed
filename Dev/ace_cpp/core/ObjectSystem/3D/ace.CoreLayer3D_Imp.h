
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

		virtual void SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size)
		{
			CoreLayer_Imp::SetLayerShape(ul, ur, ll, lr, size);
		}

		virtual void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3)
		{
			CoreLayer_Imp::AddDrawnTriangle(pos1, uv1, col1, pos2, uv2, col2, pos3, uv3, col3);
		}

		virtual void EndDrawingAfterEffects() { CoreLayer_Imp::EndDrawingAfterEffects(); }

		Renderer3D* GetRenderer() { return m_renderer; }

		RenderTexture2D* GetFirstRenderTarget();

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}