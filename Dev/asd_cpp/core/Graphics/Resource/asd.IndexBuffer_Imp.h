#pragma once

#include <Utility/asd.PointerArray.h>
#include <asd.common.Base.h>

#include "../../asd.ReferenceObject.h"
#include "../asd.DeviceObject.h"

namespace asd
{

class IndexBuffer_Imp : public DeviceObject
{
protected:
	int m_indexMaxCount;
	int m_indexCount;
	bool m_isDynamic;
	bool m_isLock;
	uint8_t* m_resource;
	bool m_is32bit;
	ar::IndexBuffer* rhi = nullptr;
	void* m_lockedResource = nullptr;

	IndexBuffer_Imp(Graphics* graphics, ar::IndexBuffer* rhi, int indexCount, bool isDynamic, bool is32bit);
	virtual ~IndexBuffer_Imp();
	void* GetBufferDirect(int count);

public:
	virtual void Lock();
	virtual void Unlock();
	int GetCount() const;
	int GetMaxCount() const;

	int32_t GetIndexSize() const;

	bool Is32Bit() const { return m_is32bit; }

	ar::IndexBuffer* GetRHI() const { return rhi; }

	static IndexBuffer_Imp* Create(Graphics* graphics, int maxCount, bool isDynamic, bool is32bit);

	/**
	@brief	内部のバッファを取得する。
	@param	count 取得する個数
	@note
	確保した以上の個数のバッファを取得した場合、assertに引っかかるので注意する必要がある。
	*/
	template <typename T> PointerArray<T> GetBuffer(int32_t count)
	{
		if (m_is32bit)
		{
			assert(sizeof(T) == 4);
		}
		else
		{
			assert(sizeof(T) == 2);
		}

		auto p = GetBufferDirect(count);
		return PointerArray<T>((T*)p, count);
	}
};

} // namespace asd