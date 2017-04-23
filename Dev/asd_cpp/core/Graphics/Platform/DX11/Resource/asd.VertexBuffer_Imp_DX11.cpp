
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.VertexBuffer_Imp_DX11.h"
#include "../asd.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	VertexBuffer_Imp_DX11::VertexBuffer_Imp_DX11(Graphics* graphics, ar::VertexBuffer* rhi, int32_t size, int32_t count, bool isDynamic)
	: VertexBuffer_Imp(graphics, size, count, isDynamic)
	, rhi(rhi)
{
	m_lockedResource = new uint8_t[count * size];
	memset(m_lockedResource, 0, (size_t) m_size * count);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_DX11::~VertexBuffer_Imp_DX11()
{
	asd::SafeDelete(rhi);
	SafeDeleteArray(m_lockedResource);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_DX11* VertexBuffer_Imp_DX11::Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic)
{
	auto g = (Graphics_Imp_DX11*) graphics;

	auto rhi = ar::VertexBuffer::Create(g->GetRHI());
	if (!rhi->Initialize(g->GetRHI(), size, count, isDynamic))
	{
		asd::SafeDelete(rhi);
		return nullptr;
	}

	return new VertexBuffer_Imp_DX11(g, rhi, size, count, isDynamic);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer_Imp_DX11::Lock()
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);

	m_vertexOffset = 0;
	m_isLock = true;

	m_resource = m_lockedResource;
	m_offset = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool VertexBuffer_Imp_DX11::RingBufferLock(int32_t count)
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);
	assert(this->m_isDynamic);

	int32_t offset = 0;
	void* data = 0;

	if (count > m_maxCount) return false;

	auto p = rhi->LockRingBuffer(count);

	m_resource = (uint8_t*)p;

	m_offset = 0;
	m_vertexOffset = 0;
	m_ringBufferLock = true;

	return true;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer_Imp_DX11::Unlock()
{
	auto g = (Graphics_Imp_DX11*) GetGraphics();

	assert(m_isLock || m_ringBufferLock);

	if (m_isLock)
	{
		if (rhi->Write(m_lockedResource, this->GetMaxCount() * this->GetSize()))
		{
		}
		else
		{
			assert(0);
		}
	}

	if (m_ringBufferLock)
	{
		rhi->Unlock();
	}

	m_resource = NULL;
	m_isLock = false;
	m_ringBufferLock = false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}