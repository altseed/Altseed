#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/asd.VertexBuffer_Imp.h"
#include "../asd.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class VertexBuffer_Imp_GL
		: public VertexBuffer_Imp
	{
	private:
		GLuint					m_buffer;

		uint32_t				m_vertexRingStart;
		uint32_t				m_vertexRingOffset;
		
		VertexBuffer_Imp_GL(Graphics* graphics, int32_t size, int32_t count, bool isDynamic);
		virtual ~VertexBuffer_Imp_GL();
	public:
		static VertexBuffer_Imp_GL* Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic);

		void Lock();
		bool RingBufferLock(int32_t count);
		void Unlock();

		GLuint GetBuffer() { return m_buffer; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}