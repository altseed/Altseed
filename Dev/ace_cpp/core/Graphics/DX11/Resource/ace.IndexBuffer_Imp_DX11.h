#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.IndexBuffer_Imp.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class IndexBuffer_Imp_DX11
		: public IndexBuffer_Imp
	{
	private:
		ID3D11Buffer*	m_buffer;
		void*			m_lockedResource;

		IndexBuffer_Imp_DX11(Graphics* graphics, ID3D11Buffer* buffer, int maxCount, bool isDynamic, bool is32bit);

	public:
		virtual ~IndexBuffer_Imp_DX11();

		static IndexBuffer_Imp_DX11* Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit);

	public:
		void Lock();
		void Unlock();

		ID3D11Buffer* GetBuffer() { return m_buffer; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}