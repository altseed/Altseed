
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
		ar::DepthTexture*	rhi = nullptr;

		DepthBuffer_Imp_DX11(Graphics* graphics, ar::DepthTexture* rhi, Vector2DI size);
		virtual ~DepthBuffer_Imp_DX11();
	public:

		static DepthBuffer_Imp_DX11* Create(Graphics* graphics, int32_t width, int32_t height);

		ar::DepthTexture* GetRHI() const { return rhi; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}