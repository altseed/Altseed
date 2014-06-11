
#include "ace.CoreScene_Imp.h"
#include "PostEffect/ace.CorePostEffect_Imp.h"

#include "../Graphics/ace.Graphics_Imp.h"
#include "../Graphics/2D/ace.LayerRenderer.h"

#include "../Graphics/2D/ace.PostEffectRenderer.h"
#include "../Graphics/Resource/ace.Shader2D_Imp.h"
#include "../Graphics/Resource/ace.Material2D_Imp.h"


using namespace std;

namespace ace
{
	void CoreScene_Imp::RenewRenderTarget(Vector2DI windowSize, bool isHDRMode)
	{
		// typeで比較するのは間違い
		auto isHDRMode_ = m_baseTarget0 != nullptr && m_baseTarget0->GetFormat() == eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT;
		auto windowSize_ = m_baseTarget0 != nullptr ? m_baseTarget0->GetSize() : Vector2DI();

		if (windowSize == windowSize_ && isHDRMode == isHDRMode_) return;

		SafeRelease(m_baseTarget0);
		SafeRelease(m_baseTarget1);

		auto format = isHDRMode ? eTextureFormat::TEXTURE_FORMAT_R32G32B32A32_FLOAT : eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM;

		m_baseTarget0 = m_graphics->CreateRenderTexture2D_Imp(windowSize.X, windowSize.Y, format);
		m_baseTarget1 = m_graphics->CreateRenderTexture2D_Imp(windowSize.X, windowSize.Y, format);


	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene_Imp::CoreScene_Imp(Graphics* graphics, Vector2DI windowSize)
		: m_layers(list<LayerPtr>())
		, m_graphics((Graphics_Imp*)graphics)
		, m_layerRenderer(nullptr)
		, m_postEffectRenderer(nullptr)
		, m_baseTarget0(nullptr)
		, m_baseTarget1(nullptr)
		, m_targetIndex(0)
		, m_windowSize(windowSize)
		, m_hdrMode(false)
	{
		SafeAddRef(m_graphics);
		
		//RenewRenderTarget(windowSize, false);

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

		m_postEffectRenderer = PostEffectRenderer::Create(graphics);
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

		SafeRelease(m_postEffectRenderer);

		SafeRelease(m_layerRenderer);

		SafeRelease(m_baseTarget0);
		SafeRelease(m_baseTarget1);
		SafeRelease(m_graphics);
	}

	bool CoreScene_Imp::GetHDRMode() const
	{
		return m_hdrMode;
	}

	void CoreScene_Imp::SetHDRMode(bool value)
	{
		m_hdrMode = value;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::AddLayer(LayerPtr layer)
	{
		m_layers.push_back(layer);
		SafeAddRef(layer);
		layer->SetScene(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::RemoveLayer(LayerPtr layer)
	{
		m_layers.remove(layer);
		layer->SetScene(nullptr);
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
		m_graphics->SetRenderTarget((RenderTexture2D_Imp*)GetBaseTarget(), nullptr);
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
	void CoreScene_Imp::BeginPostEffect(CorePostEffect* postEffect)
	{
		m_targetIndex++;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::EndPostEffect(CorePostEffect* postEffect)
	{
		auto pf = (CorePostEffect_Imp*) postEffect;

		for (auto& c : pf->GetCommands())
		{
			m_postEffectRenderer->DrawOnTexture2DWithMaterialWithCommand(c);
		}

		pf->ClearCommands();
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
		RenewRenderTarget(m_windowSize, m_hdrMode);

		m_graphics->SetRenderTarget((RenderTexture2D_Imp*) GetBaseTarget(), nullptr);
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

				positions[0].X = 0.0f;
				positions[0].Y = 0.0f;
				positions[1].X = 1.0f;
				positions[1].Y = 0.0f;
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

			positions[0].X = 0.0f;
			positions[0].Y = 1.0f;
			positions[1].X = 1.0f;
			positions[1].Y = 1.0f;
			positions[2].X = 0.0f;
			positions[2].Y = 0.0f;

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
		m_triangles.clear();
	}
}