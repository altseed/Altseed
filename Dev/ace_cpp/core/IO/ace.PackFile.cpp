#include "ace.PackFile.h"
#include "ace.Decryptor.h"
#include <vector>
#include <cassert>

namespace ace
{
	const astring PackFileHeader::Signature = ToAString("pack");

	PackFileHeader::PackFileHeader()
	{
	}

	bool PackFileHeader::Load(std::shared_ptr<BaseFile>& packedFile, const astring& key)
	{
		m_internalHeaders.clear();

		std::vector<uint8_t> buffer;
		packedFile->Seek(0);
		packedFile->ReadBytes(buffer, PackFileHeader::Signature.size(), key, packedFile->GetPosition());
		
		if (astring(reinterpret_cast<achar*>(buffer.data())).compare(PackFileHeader::Signature) == 0)
			return false;

		uint64_t fileCount = packedFile->ReadUInt64(key, packedFile->GetPosition());
		uint64_t filePathHeaderLength = packedFile->ReadUInt64(key, packedFile->GetPosition());

		packedFile->ReadBytes(buffer, filePathHeaderLength, key, packedFile->GetPosition());
		std::vector<int16_t> strBuffer;
		Utf8ToUtf16(strBuffer, reinterpret_cast<const int8_t*>(buffer.data()));

		astring str(reinterpret_cast<achar*>(strBuffer.data()));
		std::vector<int> tabIndices;
		{
			int i(0);
			for (const auto c : strBuffer)
			{
				if (i <= filePathHeaderLength)
					break;

				if (static_cast<achar>(c) == '\t')
					tabIndices.push_back(i);

				++i;
			}
		}
		std::vector<astring> ignoreFiles;
		{
			ignoreFiles.reserve(tabIndices.size() + 1);
			int currentIndex(0);

			for (const auto tabIndex : tabIndices)
			{
				ignoreFiles.push_back(str.substr(currentIndex, tabIndex + 1 - currentIndex));
			}
		}

		m_fileCount = fileCount;
		m_filePathHeaderLength = filePathHeaderLength;
		m_headerSize = sizeof(m_fileCount) + sizeof(m_headerSize) + sizeof(m_filePathHeaderLength) + m_filePathHeaderLength;

		for (int i = 0; i < fileCount; ++i)
		{
			m_internalHeaders.push_back(std::make_shared<PackFileInternalHeader>(packedFile));
		}

		return true;
	}

	std::vector<std::shared_ptr<PackFileInternalHeader>>& PackFileHeader::GetInternalHeaders()
	{ 
		return m_internalHeaders; 
	};

	PackFile::PackFile()
	{

	}

	PackFile::~PackFile()
	{

	}
	bool PackFile::Load(std::shared_ptr<BaseFile> packedFile, const astring& key)
	{
		m_InternalHeaderDictionaly.clear();

		m_TopHeader = std::make_shared<PackFileHeader>();

		if (!m_TopHeader->Load(packedFile, key))
			return false;

		auto& internalHeaders = m_TopHeader->GetInternalHeaders();

		for (auto header : internalHeaders)
		{
			m_InternalHeaderDictionaly.emplace(header->GetFileName(), header);
		}

		m_RawFile = packedFile;

		return true;
	}

	bool PackFile::HaveFile(const astring& path)
	{
		astring tmp = FileHelper::ToLower(path);
		return m_InternalHeaderDictionaly.find(tmp) != m_InternalHeaderDictionaly.end();
	}

	std::shared_ptr<PackFileHeader> PackFile::GetTopHeader()
	{
		return m_TopHeader;
	}

	std::shared_ptr<PackFileInternalHeader> PackFile::GetInternalHeader(const astring& path)
	{
		if (HaveFile(path))
		{
			astring tmp = FileHelper::ToLower(path);
			return m_InternalHeaderDictionaly[tmp];
		}
		else
		{
			return std::shared_ptr<PackFileInternalHeader>();
		}
	}
}