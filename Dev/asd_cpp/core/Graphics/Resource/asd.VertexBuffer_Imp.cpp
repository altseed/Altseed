
#include "asd.VertexBuffer_Imp.h"

#include "../asd.Graphics_Imp.h"

namespace asd
{

VertexBuffer_Imp::VertexBuffer_Imp(Graphics* graphics, ar::VertexBuffer* rhi, int32_t size, int32_t count, bool isDynamic)
	: DeviceObject(graphics)
	, m_isDynamic(isDynamic)
	, m_size(size)
	, m_maxCount(count)
	, m_offset(0)
	, m_resource(NULL)
	, m_isLock(false)
	, m_ringBufferLock(false)
	, m_vertexOffset(0)
	, rhi(rhi)
{
	auto g = (Graphics_Imp*)GetGraphics();
	g->IncVRAM(m_size * m_maxCount);
	m_lockedResource = new uint8_t[count * size];
	memset(m_lockedResource, 0, (size_t)m_size * count);
}

VertexBuffer_Imp::~VertexBuffer_Imp()
{
	auto g = (Graphics_Imp*)GetGraphics();
	g->DecVRAM(m_size * m_maxCount);

	asd::SafeDelete(rhi);
	SafeDeleteArray(m_lockedResource);
}

VertexBuffer_Imp* VertexBuffer_Imp::Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic)
{
	auto g = (Graphics_Imp*)graphics;

	auto rhi = ar::VertexBuffer::Create(g->GetRHI());
	if (!rhi->Initialize(g->GetRHI(), size, count, isDynamic))
	{
		asd::SafeDelete(rhi);
		return nullptr;
	}

	return new VertexBuffer_Imp(g, rhi, size, count, isDynamic);
}

void VertexBuffer_Imp::Lock()
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);

	m_vertexOffset = 0;
	m_isLock = true;

	m_resource = m_lockedResource;
	m_offset = 0;
}

bool VertexBuffer_Imp::RingBufferLock(int32_t count)
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);
	assert(this->m_isDynamic);

	int32_t offset = 0;
	void* data = 0;

	if (count > m_maxCount)
		return false;

	auto p = rhi->LockRingBuffer(count);

	m_resource = (uint8_t*)p;

	m_offset = 0;
	m_vertexOffset = 0;
	m_ringBufferLock = true;

	return true;
}

void VertexBuffer_Imp::Unlock()
{
	auto g = (Graphics_Imp*)GetGraphics();

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

void* VertexBuffer_Imp::GetBufferDirect(int count)
{
	assert(m_isLock || m_ringBufferLock);
	if (m_offset / m_size + count > m_maxCount)
		return NULL;

	void* pBuffer = NULL;

	/* バッファに追記 */
	pBuffer = m_resource + m_offset;
	m_offset += count * m_size;

	return pBuffer;
}

int VertexBuffer_Imp::GetMaxCount() const { return m_maxCount; }

int VertexBuffer_Imp::GetSize() const { return m_size; }

} // namespace asd