#pragma once

#include "../ace.Core.Base.h"
#include "ace.StreamFile.h"
#include "ace.BaseFile.h"
#include "ace.PackFile.h"

#include <memory>

namespace ace
{
	class StreamFile_Imp 
		: public StreamFile
		, public ReferenceObject
	{
	private:
		File_Imp*	file = nullptr;
		astring		cacheKey;

		std::shared_ptr<BaseFile>	baseFile;
		astring						key;

		int32_t						fileOffset = 0;
		int32_t						fileSize = 0;
		int32_t						current = 0;

		std::vector<uint8_t>		buffer;

	public:
		StreamFile_Imp(File_Imp* file, const astring& cacheKey, std::shared_ptr<BaseFile>& baseFile);
		StreamFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& baseFile, PackFileInternalHeader& internalHeader, const astring& key);

		virtual ~StreamFile_Imp();

		int32_t Read_(int32_t size) override;
		void* GetTempBuffer_() override;
		int32_t GetTempBufferSize_() override;

		int32_t GetSize() const override;
		int32_t GetCurrentPosition() const override;

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	};
}
