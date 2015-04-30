
#include "ace.File_Imp.h"
#include "ace.StaticFile_Imp.h"

namespace ace
{
	StaticFile_Imp::StaticFile_Imp(File_Imp* file, const astring& cacheKey, std::shared_ptr<BaseFile>& baseFile)
		: file(file)
		, cacheKey(cacheKey)
	{
		baseFile->ReadAllBytes(m_buffer);

		m_path = baseFile->GetFullPath();

		SafeAddRef(file);
	}

	StaticFile_Imp::StaticFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader, const astring& key)
		: file(file)
		, cacheKey(cacheKey)
	{
		packedFile->Seek(internalHeader.GetOffset());
		packedFile->ReadBytes(m_buffer, internalHeader.GetSize());

		if (key != astring())
		{
			std::vector<int8_t> key_temp;
			std::vector<uint8_t> key_;

			Utf16ToUtf8(key_temp, (const int16_t*)key.c_str());

			key_.resize(key_temp.size());
			memcpy(key_.data(), key_temp.data(), key_temp.size());
			key_.pop_back();

			for (size_t i = 0; i < m_buffer.size(); i++)
			{
				m_buffer[i] = (m_buffer[i] ^ key_[i % key_.size()]);
			}
		}

		m_path = packedFile->GetFullPath() + internalHeader.GetFileName();

		SafeAddRef(file);
	}

	StaticFile_Imp::~StaticFile_Imp()
	{
		file->UnregisterStaticFile(cacheKey);
		SafeRelease(file);
	}
}
