
#include "asd.File_Imp.h"
#include "asd.StreamFile_Imp.h"

namespace asd
{
	StreamFile_Imp::StreamFile_Imp(File_Imp* file, const astring& cacheKey, std::shared_ptr<BaseFile>& baseFile)
		: file(file)
		, cacheKey(cacheKey)
		, baseFile(baseFile)
	{
		fileOffset = 0;
		fileSize = baseFile->GetSize();
		current = 0;

		SafeAddRef(file);
	}

	StreamFile_Imp::StreamFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& baseFile, PackFileInternalHeader& internalHeader, std::shared_ptr<Decryptor> decryptor)
		: file(file)
		, cacheKey(cacheKey)
		, baseFile(baseFile)
		, decryptor(decryptor)
	{
		fileOffset = internalHeader.GetOffset();
		fileSize = internalHeader.GetSize();
		current = 0;

		SafeAddRef(file);
	}

	StreamFile_Imp::~StreamFile_Imp()
	{
		file->UnregisterStreamFile(cacheKey);
		SafeRelease(file);
	}

	int32_t StreamFile_Imp::Read(int32_t size)
	{
		return Read_(size);
	}

	void* StreamFile_Imp::GetTempBuffer()
	{
		return GetTempBuffer_();
	}

	int32_t StreamFile_Imp::GetTempBufferSize()
	{
		return GetTempBufferSize_();
	}

	int32_t StreamFile_Imp::Read_(int32_t size)
	{
		buffer.clear();

		int32_t readableSize = Min((fileSize - current), size);
		if (readableSize == 0) return 0;
		
		baseFile->Seek(fileOffset + current);

		baseFile->ReadBytes(buffer, readableSize, decryptor.get(), fileOffset + current);
	
		current += readableSize;

		return readableSize;
	}

	void* StreamFile_Imp::GetTempBuffer_()
	{
		return buffer.data();
	}

	int32_t StreamFile_Imp::GetTempBufferSize_()
	{
		return buffer.size();
	}

	int32_t StreamFile_Imp::GetSize() const
	{
		return fileSize;
	}

	int32_t StreamFile_Imp::GetCurrentPosition() const
	{
		return current;
	}
}
