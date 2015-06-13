
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
	VertexBuffer_Imp_DX11::VertexBuffer_Imp_DX11(Graphics* graphics, ID3D11Buffer* buffer, int32_t size, int32_t count, bool isDynamic)
	: VertexBuffer_Imp(graphics, size, count, isDynamic)
	, m_buffer(buffer)
	, m_vertexRingOffset(0)
	, m_ringLockedOffset(0)
	, m_ringLockedSize(0)
{
	m_lockedResource = new uint8_t[count * size];
	memset(m_lockedResource, 0, (size_t) m_size * count);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_DX11::~VertexBuffer_Imp_DX11()
{
	SafeDeleteArray(m_lockedResource);
	SafeRelease(m_buffer);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp_DX11* VertexBuffer_Imp_DX11::Create(Graphics* graphics, int32_t size, int32_t count, bool isDynamic)
{
	auto g = (Graphics_Imp_DX11*) graphics;

	D3D11_BUFFER_DESC hBufferDesc;
	ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));

	hBufferDesc.ByteWidth = size * count;
	hBufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hBufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float) ;

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = NULL;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;


	/* 生成 */
	ID3D11Buffer* vb = NULL;
	HRESULT hr = g->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &vb);
	if (FAILED(hr))
	{
		return NULL;
	}

	return new VertexBuffer_Imp_DX11(g, vb, size, count, isDynamic);
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

	m_resource = (uint8_t*) m_lockedResource;
	m_offset = 0;

	

	/* 次のRingBufferLockは強制的にDiscard */
	m_vertexRingOffset = m_maxCount * m_size;
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

	auto lockedSize = count * m_size;

	if (true || m_vertexRingOffset + lockedSize > m_maxCount * m_size)
	{
		offset = 0;
		m_ringLockedOffset = 0;
		m_ringLockedSize = lockedSize;

		m_vertexRingOffset = lockedSize;
	}
	else
	{
		offset = m_vertexRingOffset;
		m_ringLockedOffset = offset;
		m_ringLockedSize = lockedSize;

		m_vertexRingOffset += lockedSize;
	}

	m_offset = offset;

	data = (uint8_t*) m_lockedResource;
	m_resource = (uint8_t*) m_lockedResource;

	m_vertexOffset = offset / m_size;
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
		if (m_isDynamic)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			g->GetContext()->Map(
				m_buffer,
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&mappedResource);

			memcpy(mappedResource.pData, m_resource, m_offset);

			g->GetContext()->Unmap(m_buffer, 0);
		}
		else
		{
			g->GetContext()->UpdateSubresource(
				m_buffer,
				0,
				nullptr,
				m_resource,
				m_offset,
				0
				);
		}
	}

	if (m_ringBufferLock)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		g->GetContext()->Map(
			m_buffer,
			0,
			m_ringLockedOffset != 0 ? D3D11_MAP_WRITE_NO_OVERWRITE : D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedResource);

		uint8_t* dst = (uint8_t*) mappedResource.pData;
		dst += m_ringLockedOffset;

		uint8_t* src = (uint8_t*) m_resource;
		src += m_ringLockedOffset;

		memcpy(dst, src, m_ringLockedSize);

		g->GetContext()->Unmap(m_buffer, 0);
	}

	m_resource = NULL;
	m_isLock = false;
	m_ringBufferLock = false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}