#pragma once

#include "../common/ace.common.Base.h"
#include "ace.BaseFile_Imp.h"
#include <vector>
#include <unordered_set>

namespace ace
{
	class InternalHeader
	{
	private:
		uint32_t m_headerSize;
		uint32_t m_size;
		uint32_t m_offset;
		astring m_fileName;
		uint32_t m_fileNameLength;

	public:
		decltype(m_headerSize)		GetHeaderSize() { return m_headerSize; }
		decltype(m_size)			GetSize() { return m_size; }
		decltype(m_offset)			GetOffset() { return m_offset; }
		const decltype(m_fileName)&	GetFileName() { return m_fileName; }
		decltype(m_fileNameLength)	GetFileNameLength() { return m_fileNameLength; }

		InternalHeader(std::shared_ptr<BaseFile_Imp>& packedFile)
		{
			std::vector<uint8_t> buffer;
			std::vector<int16_t> strBuffer;

			m_fileNameLength = packedFile->ReadUInt32();
			m_size = packedFile->ReadUInt32();
			m_offset = packedFile->ReadUInt32();
			packedFile->ReadBytes(buffer, m_fileNameLength);
			Utf8ToUtf16(strBuffer, reinterpret_cast<const int8_t*>(buffer.data()));
			if (m_fileNameLength < strBuffer.size())
				strBuffer[m_fileNameLength] = 0;
			m_fileName = astring(reinterpret_cast<const achar*>(strBuffer.data()));
		}
	};

	class TopHeader
	{
	private:
		uint64_t m_headerSize;
		uint64_t m_fileCount;
		uint64_t m_filePathHeaderLength;
		std::vector<astring> m_ignoreFiles;
		std::vector<std::shared_ptr<InternalHeader>> m_internalHeaders;

	public:
		static const astring Signature;

		TopHeader();
		TopHeader(uint64_t fileCounnt, const std::vector<astring>& ignoreFiles);
		TopHeader(std::shared_ptr<BaseFile_Imp>& packedFile);

		void ToByteArray(std::vector<uint8_t>& buffer);
		void GetIgnoreFiles(std::vector<astring>& files);
		std::vector<std::shared_ptr<InternalHeader>>& GetInternalHeaders() { return m_internalHeaders; };
		void AddIgnoreFiles(std::unordered_set<astring>& files);
	};
}