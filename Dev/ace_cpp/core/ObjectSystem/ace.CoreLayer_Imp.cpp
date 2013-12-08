
#include "ace.CoreLayer_Imp.h"

#include "PostEffect/ace.CorePostEffect_Imp.h"

#include "../Graphics/Common/ace.Graphics_Imp.h"
#include "../Graphics/Common/2D/ace.LayerRenderer.h"

namespace ace
{

	CoreLayer_Imp::CoreLayer_Imp(Graphics* graphics, Vector2DI windowSize)
		: m_graphics(nullptr)
		, m_isDrawn(true)
		, m_drawingPriority(0)
		, m_renderTarget0(nullptr)
		, m_renderTarget1(nullptr)
		, m_targetToLayer(-1)
		, m_layerSize(windowSize)
		, m_windowSize(windowSize)
		, m_layerRenderer(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		m_layerRenderer = new LayerRenderer(graphics);
		m_layerRenderer->SetWindowSize(windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = windowSize.X;
			lpos[2].Y = windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}
	}

	CoreLayer_Imp::~CoreLayer_Imp()
	{
		ClearPostEffects();

		SafeRelease(m_layerRenderer);

		SafeRelease(m_renderTarget0);
		SafeRelease(m_renderTarget1);

		SafeRelease(m_graphics);
	}

	int CoreLayer_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreLayer_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

	bool CoreLayer_Imp::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void CoreLayer_Imp::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

	void CoreLayer_Imp::AddPostEffect(CorePostEffect* postEffect)
	{
		SafeAddRef(postEffect);
		m_postEffects.push_back(postEffect);

		CreateRenderTarget(m_layerSize);
	}

	void CoreLayer_Imp::ClearPostEffects()
	{
		for (auto& p : m_postEffects)
		{
			SafeRelease(p);
		}

		m_postEffects.clear();
	}

	RenderTexture2D* CoreLayer_Imp::GetRenderTarget0()
	{
		return m_renderTarget0;
	}

	RenderTexture2D* CoreLayer_Imp::GetRenderTarget1()
	{
		return m_renderTarget1;
	}

	void CoreLayer_Imp::SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size)
	{
		ace::Vector2DF lpos[4];
		lpos[0] = ul;
		lpos[1] = ur;
		lpos[2] = lr;
		lpos[3] = ll;
		m_layerRenderer->SetLayerPosition(lpos);

		CreateRenderTarget(size);
	}

	void CoreLayer_Imp::AddDrawnTriangle(
		const Vector2DF& pos1, const Vector2DF& uv1, const Color& col1,
		const Vector2DF& pos2, const Vector2DF& uv2, const Color& col2,
		const Vector2DF& pos3, const Vector2DF& uv3, const Color& col3)
	{
		Triangle t;
		t.Pos1 = pos1;
		t.UV1 = uv1;
		t.Col1 = col1;
		t.Pos2 = pos2;
		t.UV2 = uv2;
		t.Col2 = col2;
		t.Pos3 = pos3;
		t.UV3 = uv3;
		t.Col3 = col3;

		m_triangles.push_back(t);
	}


	void CoreLayer_Imp::SetTargetToLayer(int32_t index)
	{
		m_targetToLayer = index;
	}

	void CoreLayer_Imp::CreateRenderTarget(const Vector2DI& size)
	{
		if (m_layerSize == size && m_renderTarget0 != nullptr) return;
		m_layerSize = size;

		SafeRelease(m_renderTarget0);
		SafeRelease(m_renderTarget1);

		m_renderTarget0 = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		m_renderTarget1 = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
	}
}