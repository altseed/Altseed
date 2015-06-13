
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.IndexBuffer_Imp.h"

#include "../asd.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp::IndexBuffer_Imp(Graphics* graphics, int indexCount, bool isDynamic, bool is32bit)
	: DeviceObject(graphics)
	, m_indexMaxCount(indexCount)
	, m_indexCount(0)
	, m_isDynamic(false)
	, m_isLock(false)
	, m_resource(NULL)
	, m_is32bit(is32bit)
{
	auto g = (Graphics_Imp*) GetGraphics();
	g->IncVRAM(m_indexCount * (m_is32bit ? sizeof(int32_t) : sizeof(int16_t)));
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp::~IndexBuffer_Imp()
{
	auto g = (Graphics_Imp*) GetGraphics();
	g->DecVRAM(m_indexCount * (m_is32bit ? sizeof(int32_t) : sizeof(int16_t)));
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void* IndexBuffer_Imp::GetBufferDirect(int count)
{
	assert(m_isLock);
	assert(m_indexMaxCount >= m_indexCount + count);

	uint8_t* pBuffer = NULL;

	pBuffer = (uint8_t*) m_resource + (m_indexCount * GetIndexSize());
	m_indexCount += count;

	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int IndexBuffer_Imp::GetCount() const
{
	return m_indexCount;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int IndexBuffer_Imp::GetMaxCount() const
{
	return m_indexMaxCount;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int32_t IndexBuffer_Imp::GetIndexSize() const
{
	return m_is32bit ? sizeof(int32_t) : sizeof(int16_t);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}