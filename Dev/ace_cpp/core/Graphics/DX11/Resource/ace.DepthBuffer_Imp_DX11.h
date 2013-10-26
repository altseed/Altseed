
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.DepthBuffer_Imp.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	深度バッファ
		@note
		OpenGLと深度の深さが違うので注意する。
	*/
	class DepthBuffer_Imp_DX11
		: public DepthBuffer_Imp
	{
	protected:
		ID3D11Texture2D*			m_depthBuffer;
		ID3D11DepthStencilView*		m_depthStencilView;
		
		DepthBuffer_Imp_DX11(Graphics* graphics, ID3D11Texture2D* depthBuffer, ID3D11DepthStencilView* depthStencilView, Vector2DI size);
		virtual ~DepthBuffer_Imp_DX11();
	public:

		static DepthBuffer_Imp_DX11* Create(Graphics* graphics, int32_t width, int32_t height);

		ID3D11DepthStencilView* GetDepthStencilView(){ return m_depthStencilView; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}