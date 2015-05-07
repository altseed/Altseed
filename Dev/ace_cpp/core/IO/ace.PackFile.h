#pragma once

#include "ace.BaseFile.h"
#include "ace.FileHelper.h"

#include <memory>
#include <unordered_map>
#include <cassert>

namespace ace
{
	class PackFileInternalHeader
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

		PackFileInternalHeader(std::shared_ptr<BaseFile>& packedFile, const astring& key = astring())
		{
			std::vector<uint8_t> buffer;
			std::vector<int16_t> strBuffer;

			m_fileNameLength = packedFile->ReadUInt32(key, packedFile->GetPosition());
			m_size = packedFile->ReadUInt32(key, packedFile->GetPosition());
			m_offset = packedFile->ReadUInt32(key, packedFile->GetPosition());
			packedFile->ReadBytes(buffer, m_fileNameLength, key, packedFile->GetPosition());
			Utf8ToUtf16(strBuffer, reinterpret_cast<const int8_t*>(buffer.data()));
			if (m_fileNameLength < strBuffer.size())
				strBuffer[m_fileNameLength] = 0;
			m_fileName = astring(reinterpret_cast<const achar*>(strBuffer.data()));
		}
	};

	class PackFileHeader
	{
	private:
		uint64_t m_headerSize;
		uint64_t m_fileCount;
		uint64_t m_filePathHeaderLength;
		std::vector<std::shared_ptr<PackFileInternalHeader>> m_internalHeaders;

	public:
		static const astring Signature;

		PackFileHeader();

		bool Load(std::shared_ptr<BaseFile>& packedFile, const astring& key);

		std::vector<std::shared_ptr<PackFileInternalHeader>>& GetInternalHeaders();
	};

	class PackFile
	{
	private:
		std::shared_ptr<PackFileHeader> m_TopHeader;
		std::unordered_map<astring, std::shared_ptr<PackFileInternalHeader>> m_InternalHeaderDictionaly;
		std::shared_ptr<BaseFile> m_RawFile;

	public:
		decltype(m_RawFile) RawFile() { return m_RawFile; }

		PackFile();
		virtual ~PackFile();
		bool Load(std::shared_ptr<BaseFile> packedFile, const astring& key);

		bool HaveFile(const astring& path);
		std::shared_ptr<PackFileHeader> GetTopHeader();
		std::shared_ptr<PackFileInternalHeader> GetInternalHeader(const astring& path);
	};
}