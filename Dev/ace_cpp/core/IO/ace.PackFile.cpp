#include "ace.PackFile.h"
#include "ace.Decryptor.h"
#include <vector>
#include <cassert>

namespace ace
{
	const std::string PackFileHeader::Signature_old = "pack";
	const std::string PackFileHeader::Signature_new = "PACK";

	PackFileInternalHeader::PackFileInternalHeader(std::shared_ptr<BaseFile>& packedFile, Decryptor* decryptor)
	{
		std::vector<uint8_t> buffer;
		std::vector<int16_t> strBuffer;

		m_fileNameLength = packedFile->ReadUInt32(decryptor, packedFile->GetPosition());
		m_size = packedFile->ReadUInt32(decryptor, packedFile->GetPosition());
		m_offset = packedFile->ReadUInt32(decryptor, packedFile->GetPosition());
		packedFile->ReadBytes(buffer, m_fileNameLength, decryptor, packedFile->GetPosition());
		Utf8ToUtf16(strBuffer, reinterpret_cast<const int8_t*>(buffer.data()));

		if (m_fileNameLength < strBuffer.size())
		{
			strBuffer[m_fileNameLength] = 0;
		}
			
		m_fileName = astring(reinterpret_cast<const achar*>(strBuffer.data()));
	}

	PackFileHeader::PackFileHeader()
	{
	}

	bool PackFileHeader::Load(std::shared_ptr<BaseFile>& packedFile, Decryptor* decryptor)
	{
		m_internalHeaders.clear();

		std::vector<uint8_t> buffer_sig_old;
		std::vector<uint8_t> buffer_sig_new;

		packedFile->Seek(0);
		packedFile->ReadBytes(buffer_sig_old, PackFileHeader::Signature_old.size(), decryptor, packedFile->GetPosition());
		
		packedFile->Seek(0);
		packedFile->ReadBytes(buffer_sig_new, PackFileHeader::Signature_new.size(), nullptr, packedFile->GetPosition());

		buffer_sig_old.push_back(0);
		buffer_sig_new.push_back(0);

		version = 0;

		auto compared_old = std::string(reinterpret_cast<char*>(buffer_sig_old.data())).compare(PackFileHeader::Signature_old);
		auto compared_new = std::string(reinterpret_cast<char*>(buffer_sig_new.data())).compare(PackFileHeader::Signature_new);

		if (compared_old == 0)
		{
			version = 0;
		}
		else if (compared_new == 0)
		{
			version = 1;
		}
		else
		{
			return false;
		}

		if (version >= 1)
		{
			std::vector<uint8_t> buffer_;
			version = packedFile->ReadUInt32(nullptr, packedFile->GetPosition());
			
			if (decryptor != nullptr)
			{
				decryptor->ChangeVersion(version);
			}

			packedFile->ReadBytes(buffer_, PackFileHeader::Signature_new.size(), nullptr, packedFile->GetPosition());
			buffer_.push_back(0);

			auto compared = std::string(reinterpret_cast<char*>(buffer_.data())).compare(PackFileHeader::Signature_new);
			if (compared_new != 0) return false;
		}

		std::vector<uint8_t> buffer;

		uint64_t fileCount = packedFile->ReadUInt64(decryptor, packedFile->GetPosition());
		uint64_t filePathHeaderLength = packedFile->ReadUInt64(decryptor, packedFile->GetPosition());

		std::vector<int16_t> strBuffer;
		if (filePathHeaderLength > 0)
		{
			packedFile->ReadBytes(buffer, filePathHeaderLength, decryptor, packedFile->GetPosition());
			Utf8ToUtf16(strBuffer, reinterpret_cast<const int8_t*>(buffer.data()));
		}
		else
		{
			strBuffer.push_back(0);
		}
		
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
			m_internalHeaders.push_back(std::make_shared<PackFileInternalHeader>(packedFile, decryptor));
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
	bool PackFile::Load(std::shared_ptr<BaseFile> packedFile, Decryptor* decryptor)
	{
		m_InternalHeaderDictionaly.clear();

		m_TopHeader = std::make_shared<PackFileHeader>();

		if (!m_TopHeader->Load(packedFile, decryptor))
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