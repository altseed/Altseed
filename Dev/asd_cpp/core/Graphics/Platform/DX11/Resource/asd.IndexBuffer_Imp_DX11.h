#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.IndexBuffer_Imp.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class IndexBuffer_Imp_DX11
		: public IndexBuffer_Imp
	{
	private:
		ar::IndexBuffer*					rhi = nullptr;

		//ID3D11Buffer*	m_buffer;
		void*			m_lockedResource;

		IndexBuffer_Imp_DX11(Graphics* graphics, ar::IndexBuffer* rhi, ID3D11Buffer* buffer, int maxCount, bool isDynamic, bool is32bit);

	public:
		virtual ~IndexBuffer_Imp_DX11();

		static IndexBuffer_Imp_DX11* Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit);

	public:
		void Lock();
		void Unlock();

		//ID3D11Buffer* GetBuffer() { return m_buffer; }

		ar::IndexBuffer* GetRHI() const { return rhi; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}