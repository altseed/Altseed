
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.DepthBuffer_Imp_DX11.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11::DepthBuffer_Imp_DX11(Graphics* graphics, ID3D11Texture2D* depthBuffer, ID3D11DepthStencilView* depthStencilView, Vector2DI size)
		: DepthBuffer_Imp(graphics)
		, m_depthBuffer(depthBuffer)
		, m_depthStencilView(depthStencilView)
	{
		m_size = size;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11::~DepthBuffer_Imp_DX11()
	{
		SafeRelease(m_depthBuffer);
		SafeRelease(m_depthStencilView);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11* DepthBuffer_Imp_DX11::Create(Graphics* graphics, int32_t width, int32_t height)
	{
		auto g = (Graphics_Imp_DX11*)graphics;

		ID3D11Texture2D* depthBuffer = nullptr;
		ID3D11DepthStencilView* depthStencilView = nullptr;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		
		if(FAILED(g->GetDevice()->CreateTexture2D(&desc, NULL, &depthBuffer)))
		{
			goto End;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format			= desc.Format;
		viewDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		viewDesc.Flags			= 0;
		if (FAILED(g->GetDevice()->CreateDepthStencilView(depthBuffer, &viewDesc, &depthStencilView)))
		{
			goto End;
		}
		

		return new DepthBuffer_Imp_DX11(graphics, depthBuffer, depthStencilView, Vector2DI(width, height));

	End:;
		SafeRelease(depthBuffer);
		SafeRelease(depthStencilView);
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}