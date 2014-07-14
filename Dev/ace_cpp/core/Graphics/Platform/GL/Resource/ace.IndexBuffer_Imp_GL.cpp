
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.IndexBuffer_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	IndexBuffer_Imp_GL::IndexBuffer_Imp_GL(Graphics* graphics, GLuint buffer, int maxCount, bool isDynamic, bool is32bit)
		: IndexBuffer_Imp(graphics, maxCount, isDynamic, is32bit)
		, m_buffer(buffer)
		, m_lockedResource(nullptr)
	{
		m_lockedResource = new uint8_t[m_indexMaxCount * GetIndexSize()];
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	IndexBuffer_Imp_GL::~IndexBuffer_Imp_GL()
	{
		SafeDeleteArray(m_lockedResource);
		glDeleteBuffers(1, &m_buffer);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	IndexBuffer_Imp_GL* IndexBuffer_Imp_GL::Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit)
	{
		GLuint ib;
		glGenBuffers(1, &ib);
		GLCheckError();

		return new IndexBuffer_Imp_GL(graphics, ib, maxCount, isDynamic, is32bit);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void IndexBuffer_Imp_GL::Lock()
	{
		assert(!m_isLock);

		m_isLock = true;
		m_resource = (uint8_t*) m_lockedResource;
		m_indexCount = 0;
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	void IndexBuffer_Imp_GL::Unlock()
	{
		assert(m_isLock);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * GetIndexSize(), m_resource, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_isLock = false;

		GLCheckError();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}