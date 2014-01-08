
#include "ace.CoreScene_Imp.h"

#include "../Graphics/Common/ace.Graphics_Imp.h"
#include "../Graphics/Common/2D/ace.LayerRenderer.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene_Imp::CoreScene_Imp(Graphics* graphics, Vector2DI windowSize)
		: m_layers(list<LayerPtr>())
		, m_graphics((Graphics_Imp*)graphics)
		, m_layerRenderer(nullptr)
		, m_baseTarget0(nullptr)
		, m_baseTarget1(nullptr)
		, m_targetIndex(0)

	{
		SafeAddRef(m_graphics);
		m_baseTarget0 = m_graphics->CreateRenderTexture_Imp(windowSize.X, windowSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		m_baseTarget1 = m_graphics->CreateRenderTexture_Imp(windowSize.X, windowSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);

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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene_Imp::~CoreScene_Imp()
	{
		for (auto& layer : m_layers)
		{
			SafeRelease(layer);
		}

		SafeRelease(m_layerRenderer);

		SafeRelease(m_baseTarget0);
		SafeRelease(m_baseTarget1);
		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::AddLayer(LayerPtr layer)
	{
		m_layers.push_back(layer);
		SafeAddRef(layer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::RemoveLayer(LayerPtr layer)
	{
		m_layers.remove(layer);
		SafeRelease(layer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::Draw()
	{
		m_layers.sort([](CoreLayer* x, CoreLayer* y)
		{
			return x->GetDrawingPriority() < y->GetDrawingPriority();
		});

		for (auto& layer : m_layers)
		{
			layer->Draw();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::SetRenderTargetForDrawingLayer()
	{
		m_graphics->SetRenderTarget((RenderTexture_Imp*)GetBaseTarget(), nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreScene_Imp::GetBaseTarget()
	{
		if (m_targetIndex % 2 == 0) return m_baseTarget0;
		return m_baseTarget1;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::BeginPostEffect()
	{
		m_targetIndex++;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::EndPostEffect()
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreScene_Imp::GetSrcTarget()
	{
		if (m_targetIndex % 2 == 1) return m_baseTarget0;
		return m_baseTarget1;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreScene_Imp::GetDstTarget()
	{
		if (m_targetIndex % 2 == 0) return m_baseTarget0;
		return m_baseTarget1;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::SetLayerShape(const Vector2DF& ul, const Vector2DF& ur, const Vector2DF& ll, const Vector2DF& lr, const Vector2DI& size)
	{
		ace::Vector2DF lpos[4];
		lpos[0] = ul;
		lpos[1] = ur;
		lpos[2] = lr;
		lpos[3] = ll;
		m_layerRenderer->SetLayerPosition(lpos);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::AddDrawnTriangle(
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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::BeginDrawing()
	{
		m_graphics->SetRenderTarget((RenderTexture_Imp*) GetBaseTarget(), nullptr);
		m_graphics->Clear(true, false, Color(0, 0, 0, 255));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::EndDrawing()
	{
		m_graphics->SetRenderTarget(nullptr, nullptr);

		m_layerRenderer->SetTexture(GetBaseTarget());

		if (m_triangles.size() > 0)
		{
			for (auto& t : m_triangles)
			{
				ace::Vector2DF positions[4];
				ace::Color colors[4];
				ace::Vector2DF uvs[4];

				colors[0] = t.Col1;
				colors[1] = t.Col2;
				colors[2] = t.Col3;

				positions[0] = t.Pos1;
				positions[1] = t.Pos2;
				positions[2] = t.Pos3;

				uvs[0] = t.UV1;
				uvs[1] = t.UV2;
				uvs[2] = t.UV3;

				m_layerRenderer->AddTriangle(positions, colors, uvs);
			}
		}
		else
		{
			{
				ace::Vector2DF positions[4];
				ace::Color colors[4];
				ace::Vector2DF uvs[4];

				colors[0] = ace::Color(255, 255, 255, 255);
				colors[1] = ace::Color(255, 255, 255, 255);
				colors[2] = ace::Color(255, 255, 255, 255);

				positions[0].X = -1.0f;
				positions[0].Y = -1.0f;
				positions[1].X = 1.0f;
				positions[1].Y = -1.0f;
				positions[2].X = 1.0f;
				positions[2].Y = 1.0f;


				uvs[0].X = 0;
				uvs[0].Y = 0;
				uvs[1].X = 1;
				uvs[1].Y = 0;
				uvs[2].X = 1;
				uvs[2].Y = 1;

				m_layerRenderer->AddTriangle(positions, colors, uvs);
			}

			{
			ace::Vector2DF positions[4];
			ace::Color colors[4];
			ace::Vector2DF uvs[4];

			colors[0] = ace::Color(255, 255, 255, 255);
			colors[1] = ace::Color(255, 255, 255, 255);
			colors[2] = ace::Color(255, 255, 255, 255);

			positions[0].X = -1.0f;
			positions[0].Y = 1.0f;
			positions[1].X = 1.0f;
			positions[1].Y = 1.0f;
			positions[2].X = -1.0f;
			positions[2].Y = -1.0f;

			uvs[0].X = 0;
			uvs[0].Y = 1;
			uvs[1].X = 1;
			uvs[1].Y = 1;
			uvs[2].X = 0;
			uvs[2].Y = 0;

			m_layerRenderer->AddTriangle(positions, colors, uvs);
		}
		}

		m_layerRenderer->DrawCache();
		m_layerRenderer->ClearCache();
	}
}