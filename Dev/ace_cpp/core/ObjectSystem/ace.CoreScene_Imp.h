#pragma once
#include <list>
#include <memory>
#include "ace.CoreScene.h"

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>

namespace ace
{
	class CoreScene_Imp
		: public CoreScene
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
		LayerRenderer*			m_layerRenderer;
		PostEffectRenderer*		m_postEffectRenderer;

		std::list<LayerPtr> m_layers;

		Graphics_Imp*		m_graphics;
		RenderTexture_Imp*	m_baseTarget0;
		RenderTexture_Imp*	m_baseTarget1;
		int32_t				m_targetIndex;

		CoreScene_Imp(Graphics* graphics, Vector2DI windowSize);
		virtual ~CoreScene_Imp();
	public:
		
		void AddLayer(LayerPtr layer);
		void RemoveLayer(LayerPtr layer);
		
		void Draw();

		void SetRenderTargetForDrawingLayer();

		RenderTexture2D* GetBaseTarget();
		void BeginPostEffect(CorePostEffect* postEffect);
		void EndPostEffect(CorePostEffect* postEffect);
		RenderTexture2D* GetSrcTarget();
		RenderTexture2D* GetDstTarget();

		void AddDrawnTriangle(
			const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
			const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
			const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3);

		void BeginDrawing();

		void EndDrawing();

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}