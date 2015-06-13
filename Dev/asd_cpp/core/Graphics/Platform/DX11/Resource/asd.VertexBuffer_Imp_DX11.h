#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.VertexBuffer_Imp.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class VertexBuffer_Imp_DX11
		: public VertexBuffer_Imp
	{
	private:

		ID3D11Buffer*			m_buffer;
		void*					m_lockedResource;

		uint32_t				m_vertexRingOffset;

		int32_t					m_ringLockedOffset;
		int32_t					m_ringLockedSize;


		VertexBuffer_Imp_DX11(Graphics* graphics, ID3D11Buffer* buffer, int32_t size, int32_t count, bool isDynamic);
		virtual ~VertexBuffer_Imp_DX11();
	public:

		static VertexBuffer_Imp_DX11* Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic);

		void Lock();
		bool RingBufferLock(int32_t count);
		void Unlock();

		ID3D11Buffer* GetBuffer() { return m_buffer; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}