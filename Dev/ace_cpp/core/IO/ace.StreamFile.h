#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"

namespace ace
{
	class StreamFile
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual int32_t Read_(int32_t size) = 0;
		virtual void* GetTempBuffer_() = 0;
		virtual int32_t GetTempBufferSize_() = 0;

	public:
		StreamFile() {}
		virtual ~StreamFile() { };
		
		virtual int32_t GetSize() const = 0;

		virtual int32_t GetCurrentPosition() const = 0;

#if !SWIG
		void Read(std::vector<byte>& buffer, int32_t size)
		{
			auto result = Read_(size);
			buffer.resize(result);
			memcpy(buffer.data(), GetTempBuffer_(), result);
		}
#endif
	};
}