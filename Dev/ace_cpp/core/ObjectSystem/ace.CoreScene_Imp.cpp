
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
		, m_postEffectRenderer(nullptr)
		, m_baseTarget0(nullptr)
		, m_baseTarget1(nullptr)
		, m_targetIndex(0)
		, m_windowSize(windowSize)
		, m_hdrMode(false)
	{
		SafeAddRef(m_graphics);
		
		//RenewRenderTarget(windowSize, false);

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
	}
}