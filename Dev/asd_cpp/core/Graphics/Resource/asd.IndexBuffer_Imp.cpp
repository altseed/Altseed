
#include "asd.IndexBuffer_Imp.h"
#include "../asd.Graphics_Imp.h"

namespace asd
{

IndexBuffer_Imp::IndexBuffer_Imp(Graphics* graphics, ar::IndexBuffer* rhi, int indexCount, bool isDynamic, bool is32bit)
	: DeviceObject(graphics)
	, rhi(rhi)
	, m_indexMaxCount(indexCount)
	, m_indexCount(0)
	, m_isDynamic(false)
	, m_isLock(false)
	, m_resource(NULL)
	, m_is32bit(is32bit)
{
	auto g = (Graphics_Imp*)GetGraphics();
	g->IncVRAM(m_indexMaxCount * (m_is32bit ? sizeof(int32_t) : sizeof(int16_t)));
	m_lockedResource = new uint8_t[GetIndexSize() * indexCount];
}

IndexBuffer_Imp::~IndexBuffer_Imp()
{
	SafeDeleteArray(m_lockedResource);
	auto g = (Graphics_Imp*)GetGraphics();
	g->DecVRAM(m_indexMaxCount * (m_is32bit ? sizeof(int32_t) : sizeof(int16_t)));
	asd::SafeDelete(rhi);
}

void* IndexBuffer_Imp::GetBufferDirect(int count)
{
	assert(m_isLock);
	assert(m_indexMaxCount >= m_indexCount + count);

	uint8_t* pBuffer = NULL;

	pBuffer = (uint8_t*)m_resource + (m_indexCount * GetIndexSize());
	m_indexCount += count;

	return pBuffer;
}

void IndexBuffer_Imp::Lock()
{
	assert(!m_isLock);

	m_isLock = true;
	m_resource = (uint8_t*)m_lockedResource;
	m_indexCount = 0;
}

void IndexBuffer_Imp::Unlock()
{
	auto g = (Graphics_Imp*)GetGraphics();

	assert(m_isLock);

	rhi->Write(m_resource, GetIndexSize() * GetMaxCount());

	m_resource = NULL;
	m_isLock = false;
}

int IndexBuffer_Imp::GetCount() const { return m_indexCount; }

int IndexBuffer_Imp::GetMaxCount() const { return m_indexMaxCount; }

int32_t IndexBuffer_Imp::GetIndexSize() const { return m_is32bit ? sizeof(int32_t) : sizeof(int16_t); }

IndexBuffer_Imp* IndexBuffer_Imp::Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit)
{
	auto g = (Graphics_Imp*)graphics;

	auto rhi = ar::IndexBuffer::Create(g->GetRHI());
	if (rhi->Initialize(g->GetRHI(), maxCount, is32bit))
	{
		return new IndexBuffer_Imp(g, rhi, maxCount, isDynamic, is32bit);
	}

	asd::SafeDelete(rhi);

	return nullptr;
}

} // namespace asd