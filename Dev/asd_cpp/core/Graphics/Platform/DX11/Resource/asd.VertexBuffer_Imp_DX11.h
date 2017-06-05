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
		ar::VertexBuffer*					rhi = nullptr;
		uint8_t*				m_lockedResource;

		VertexBuffer_Imp_DX11(Graphics* graphics, ar::VertexBuffer* rhi, int32_t size, int32_t count, bool isDynamic);
		virtual ~VertexBuffer_Imp_DX11();
	public:

		static VertexBuffer_Imp_DX11* Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic);

		void Lock();
		bool RingBufferLock(int32_t count);
		void Unlock();

		ar::VertexBuffer* GetRHI() const { return rhi; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}