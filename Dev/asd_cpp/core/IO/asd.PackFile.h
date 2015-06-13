#pragma once

#include "asd.BaseFile.h"
#include "asd.FileHelper.h"
#include "asd.Decryptor.h"

#include <memory>
#include <unordered_map>
#include <cassert>

namespace asd
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

		PackFileInternalHeader(std::shared_ptr<BaseFile>& packedFile, Decryptor* decryptor = nullptr);
	};

	class PackFileHeader
	{
	private:
		uint64_t m_headerSize;
		uint64_t m_fileCount;
		uint64_t m_filePathHeaderLength;
		uint32_t version = 0;

		std::vector<std::shared_ptr<PackFileInternalHeader>> m_internalHeaders;

	public:
		static const std::string Signature_old;
		static const std::string Signature_new;

		PackFileHeader();

		bool Load(std::shared_ptr<BaseFile>& packedFile, Decryptor* decryptor);

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
		bool Load(std::shared_ptr<BaseFile> packedFile, Decryptor* decryptor);

		bool HaveFile(const astring& path);
		std::shared_ptr<PackFileHeader> GetTopHeader();
		std::shared_ptr<PackFileInternalHeader> GetInternalHeader(const astring& path);
	};
}