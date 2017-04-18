
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
IndexBuffer_Imp_DX11::IndexBuffer_Imp_DX11(Graphics* graphics, ar::IndexBuffer* rhi, int maxCount, bool isDynamic, bool is32bit)
	: IndexBuffer_Imp(graphics, maxCount, isDynamic, is32bit)
	, rhi(rhi)
	, m_lockedResource(NULL)
{
	m_lockedResource = new uint8_t[GetIndexSize() * maxCount];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer_Imp_DX11::~IndexBuffer_Imp_DX11()
{
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
		return new IndexBuffer_Imp_DX11(g, rhi, maxCount, isDynamic, is32bit);
	}

	asd::SafeDelete(rhi);

	return nullptr;
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

	rhi->Write(m_resource, GetIndexSize() * GetMaxCount());

	m_resource = NULL;
	m_isLock = false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}