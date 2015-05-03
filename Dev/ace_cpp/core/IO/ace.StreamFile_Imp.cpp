
#include "ace.File_Imp.h"
#include "ace.StreamFile_Imp.h"

namespace ace
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

	StreamFile_Imp::StreamFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& baseFile, PackFileInternalHeader& internalHeader, const astring& key)
		: file(file)
		, cacheKey(cacheKey)
		, baseFile(baseFile)
		, key(key)
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

	int32_t StreamFile_Imp::Read_(int32_t size)
	{
		buffer.clear();

		int32_t readableSize = Min((fileSize - current), size);
		if (readableSize == 0) return 0;
		
		baseFile->Seek(fileOffset + current);
		baseFile->ReadBytes(buffer, readableSize);

		if (key != astring())
		{
			std::vector<int8_t> key_temp;
			std::vector<uint8_t> key_;

			Utf16ToUtf8(key_temp, (const int16_t*) key.c_str());

			key_.resize(key_temp.size());
			memcpy(key_.data(), key_temp.data(), key_temp.size());
			key_.pop_back();

			for (size_t i = 0; i < buffer.size(); i++)
			{
				buffer[i] = (buffer[i] ^ key_[(i + current) % key_.size()]);
			}
		}

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
