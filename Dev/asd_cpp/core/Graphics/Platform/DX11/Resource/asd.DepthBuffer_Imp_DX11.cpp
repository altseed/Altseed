
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.DepthBuffer_Imp_DX11.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11::DepthBuffer_Imp_DX11(Graphics* graphics, ID3D11Texture2D* depthBuffer, ID3D11DepthStencilView* depthStencilView, ID3D11ShaderResourceView* depthSRV, Vector2DI size)
		: DepthBuffer_Imp(graphics, size)
		, m_depthBuffer(depthBuffer)
		, m_depthStencilView(depthStencilView)
		, m_depthSRV(depthSRV)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11::~DepthBuffer_Imp_DX11()
	{
		SafeRelease(m_depthBuffer);
		SafeRelease(m_depthStencilView);
		SafeRelease(m_depthSRV);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_DX11* DepthBuffer_Imp_DX11::Create(Graphics* graphics, int32_t width, int32_t height)
	{
		auto g = (Graphics_Imp_DX11*)graphics;

		ID3D11Texture2D* depthBuffer = nullptr;
		ID3D11DepthStencilView* depthStencilView = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		
		if(FAILED(g->GetDevice()->CreateTexture2D(&desc, NULL, &depthBuffer)))
		{
			goto End;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		viewDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		viewDesc.Flags			= 0;
		if (FAILED(g->GetDevice()->CreateDepthStencilView(depthBuffer, &viewDesc, &depthStencilView)))
		{
			goto End;
		}
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		if (FAILED(g->GetDevice()->CreateShaderResourceView(depthBuffer, &srvDesc, &srv)))
		{
			goto End;
		}

		return new DepthBuffer_Imp_DX11(graphics, depthBuffer, depthStencilView, srv, Vector2DI(width, height));

	End:;
		SafeRelease(depthBuffer);
		SafeRelease(depthStencilView);
		SafeRelease(srv);
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}