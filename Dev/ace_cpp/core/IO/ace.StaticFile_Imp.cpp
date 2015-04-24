#include "ace.StaticFile_Imp.h"

namespace ace
{
	StaticFile_Imp::~StaticFile_Imp()
	{
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile>& file)
	{
		file->ReadAllBytes(m_buffer);

		m_path = file->GetFullPath();
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader, const astring& key)
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
	}
}
