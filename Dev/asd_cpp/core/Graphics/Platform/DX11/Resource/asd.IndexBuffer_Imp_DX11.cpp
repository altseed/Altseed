
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.IndexBuffer_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp_DX11::IndexBuffer_Imp_DX11(Graphics* graphics, ar::IndexBuffer* rhi, ID3D11Buffer* buffer, int maxCount, bool isDynamic, bool is32bit)
	: IndexBuffer_Imp(graphics, maxCount, isDynamic, is32bit)
	, rhi(rhi)
	//, m_buffer(buffer)
	, m_lockedResource(NULL)
{
	m_lockedResource = new uint8_t[GetIndexSize() * maxCount];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp_DX11::~IndexBuffer_Imp_DX11()
{
	//SafeRelease(m_buffer);
	SafeDeleteArray(m_lockedResource);
	asd::SafeDelete(rhi);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp_DX11* IndexBuffer_Imp_DX11::Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit)
{
	auto g = (Graphics_Imp_DX11*)graphics;

	auto rhi = ar::IndexBuffer::Create(g->GetRHI());
	if (rhi->Initialize(g->GetRHI(), maxCount, is32bit))
	{
		return new IndexBuffer_Imp_DX11(g, rhi, nullptr, maxCount, isDynamic, is32bit);
	}

	asd::SafeDelete(rhi);

	return nullptr;

	/*
	auto indSize = is32bit ? sizeof(int32_t) : sizeof(int16_t);

	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	hBufferDesc.ByteWidth = indSize * maxCount;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = indSize;

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = NULL;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	// 生成
	ID3D11Buffer* ib = NULL;
	if (FAILED(g->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &ib)))
	{
		return NULL;
	}

	return new IndexBuffer_Imp_DX11(g, ib, maxCount, isDynamic, is32bit);
	*/
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer_Imp_DX11::Lock()
{
	assert(!m_isLock);

	m_isLock = true;
	m_resource = (uint8_t*) m_lockedResource;
	m_indexCount = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer_Imp_DX11::Unlock()
{
	auto g = (Graphics_Imp_DX11*) GetGraphics();

	assert(m_isLock);

	rhi->Write(rhi, GetIndexSize() * GetMaxCount());
	/*
	if (m_isDynamic)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		g->GetContext()->Map(
			m_buffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedResource);

		memcpy(mappedResource.pData, m_resource, m_indexCount * GetMaxCount());

		g->GetContext()->Unmap(m_buffer, 0);
	}
	else
	{
		g->GetContext()->UpdateSubresource(
			m_buffer,
			0,
			NULL,
			m_resource,
			m_indexCount * GetMaxCount(),
			0);
	}
	*/

	m_resource = NULL;
	m_isLock = false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}