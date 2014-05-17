#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/ace.RenderState_Imp.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderState_Imp_DX11
		: public RenderState_Imp
	{
	private:
		static const int32_t		DepthTestCount = 2;
		static const int32_t		DepthWriteCount = 2;
		static const int32_t		CulTypeCount = 3;
		static const int32_t		AlphaTypeCount = 5;
		static const int32_t		TextureFilterCount = 2;
		static const int32_t		TextureWrapCount = 2;

		Graphics_Imp_DX11*			m_graphics;
		ID3D11RasterizerState*		m_rStates[CulTypeCount];
		ID3D11DepthStencilState*	m_dStates[DepthTestCount][DepthWriteCount];
		ID3D11BlendState*			m_bStates[AlphaTypeCount];
		ID3D11SamplerState*			m_sStates[TextureFilterCount][TextureWrapCount];

	public:
		RenderState_Imp_DX11(Graphics_Imp_DX11* graphics);
		virtual ~RenderState_Imp_DX11();

		void Update(bool forced);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}