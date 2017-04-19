
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.DepthBuffer_Imp.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	深度バッファ
	*/
	class DepthBuffer_Imp_DX11
		: public DepthBuffer_Imp
	{
	protected:
		ar::DepthTexture*					rhi = nullptr;

		ID3D11Texture2D*			m_depthBuffer;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11ShaderResourceView*	m_depthSRV;

		DepthBuffer_Imp_DX11(Graphics* graphics, ID3D11Texture2D* depthBuffer, ID3D11DepthStencilView* depthStencilView, ID3D11ShaderResourceView* depthSRV, Vector2DI size);
		virtual ~DepthBuffer_Imp_DX11();
	public:

		static DepthBuffer_Imp_DX11* Create(Graphics* graphics, int32_t width, int32_t height);

		ID3D11DepthStencilView* GetDepthStencilView(){ return m_depthStencilView; }

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_depthSRV; }

		ar::DepthTexture* GetRHI() const { return rhi; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}