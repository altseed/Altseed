#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include <Utility/ace.PointerArray.h>

#include "../../../ace.ReferenceObject.h"
#include "../ace.DeviceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class VertexBuffer_Imp
		: public DeviceObject
	{
	private:

	protected:
		bool					m_isDynamic;
		int						m_size;
		int32_t					m_maxCount;
		int						m_offset;
		uint8_t*				m_resource;
		bool					m_isLock;
		bool					m_ringBufferLock;

		int32_t					m_vertexOffset;


		VertexBuffer_Imp(Graphics* graphics, int32_t size, int32_t count, bool isDynamic);
		virtual ~VertexBuffer_Imp();
		void* GetBufferDirect(int count);
	public:

		virtual void Lock() = 0;
		virtual bool RingBufferLock(int32_t count) = 0;
		virtual void Unlock() = 0;
		virtual int GetMaxCount() const;
		virtual int GetSize() const;
		
		/**
			@brief	前回ロックした時のデータが何番目の頂点から存在しているか取得する。
		*/
		int32_t GetVertexOffset() { return m_vertexOffset; }
		
		/**
			@brief	内部のバッファを取得する。
			@param	count 取得する個数
			@note
			確保した以上の個数のバッファを取得した場合、assertに引っかかるので注意する必要がある。
		*/
		template<typename T>
		PointerArray<T> GetBuffer(int32_t count)
		{
			assert(sizeof(T) == GetSize());
			auto p = GetBufferDirect(count);
			return PointerArray<T>((T*)p, count);
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}