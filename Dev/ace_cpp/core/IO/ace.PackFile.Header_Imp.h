#pragma once

#include "../common/ace.common.Base.h"
#include <vector>

namespace ace
{
	class TopHeader
	{
	private:
		static const astring Signature;

		uint64_t m_headerSize;
		uint64_t m_fileCount;
		uint64_t m_filePathHeaderLength;
		std::vector<astring> m_ignoreFiles;

	public:
		TopHeader();
		TopHeader(uint64_t fileCounnt, const std::vector<astring>& const ignoreFiles);

		void ToByteArray(std::vector<uint8_t>& buffer);
		void GetIgnoreFiles(std::vector<astring>& files);
		void AddIgnoreFiles(std::vector<astring>& files);
	};

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

		InternalHeader();
	};
}