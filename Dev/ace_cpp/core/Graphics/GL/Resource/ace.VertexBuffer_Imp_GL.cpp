
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.VertexBuffer_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_GL::VertexBuffer_Imp_GL(Graphics* graphics, int32_t size, int32_t count, bool isDynamic)
	: VertexBuffer_Imp(graphics, size, count, isDynamic)
	, m_vertexRingStart(0)
	, m_vertexRingOffset(0)
{
	m_resource = new uint8_t[m_size * count];
	memset(m_resource, 0, (size_t) m_size * count);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_size * count, m_resource, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBuffer_Imp_GL::~VertexBuffer_Imp_GL()
{
	glDeleteBuffers(1, &m_buffer);
	SafeDeleteArray(m_resource);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_GL* VertexBuffer_Imp_GL::Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic)
{
	auto g = (Graphics_Imp_GL*) graphics;

	return new VertexBuffer_Imp_GL(g, size, count, isDynamic);
}


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer_Imp_GL::Lock()
{
	assert(!m_isLock);

	m_vertexOffset = 0;
	m_isLock = true;
	m_offset = 0;
	m_vertexRingStart = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool VertexBuffer_Imp_GL::RingBufferLock(int32_t count)
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);
	assert(this->m_isDynamic);

	int32_t offset = 0;
	void* data = 0;

	if (count > m_maxCount) return false;

	auto lockedSize = count * m_size;

	//if ( m_vertexRingOffset + size > m_size )
	if (true)
	{
		offset = 0;
		data = m_resource;
		m_vertexRingOffset = lockedSize;
		m_offset = offset;

		m_vertexRingStart = offset;
	}
	else
	{
		offset = m_vertexRingOffset;
		data = m_resource;
		m_offset = offset;
		m_vertexRingOffset += lockedSize;

		m_vertexRingStart = offset;
	}

	m_vertexOffset = offset / m_size;
	m_ringBufferLock = true;

	return true;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer_Imp_GL::Unlock()
{
	std::lock_guard<std::recursive_mutex> lock(((Graphics_Imp_GL*) GetGraphics())->GetMutex());
	((Graphics_Imp_GL*) GetGraphics())->MakeContextCurrent();

	assert(m_isLock || m_ringBufferLock);

	GLCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	GLCheckError();

	glBufferSubData(GL_ARRAY_BUFFER, m_vertexRingStart, m_offset, m_resource);
	GLCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLCheckError();

	m_isLock = false;
	m_ringBufferLock = false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}