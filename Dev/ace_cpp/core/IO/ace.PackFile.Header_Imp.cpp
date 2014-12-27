#include "ace.FileUtility.h"
#include "ace.PackFile.Header_Imp.h"
#include "../common/ace.common.Base.h"

namespace ace
{
	TopHeader::TopHeader(std::shared_ptr<BaseFile_Imp>& packedFile)
	{
		std::vector<uint8_t> buffer;
		packedFile->Seek(0);
		packedFile->ReadBytes(buffer, TopHeader::Signature.size());
		assert(ace::ToAString(reinterpret_cast<achar*>(buffer.data())).compare(TopHeader::Signature) != 0);

		uint64_t fileCount = packedFile->ReadUInt64();
		uint64_t filePathHeaderLength = packedFile->ReadUInt64();

		packedFile->ReadBytes(buffer, filePathHeaderLength);
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
		m_ignoreFiles = ignoreFiles;
		m_filePathHeaderLength = filePathHeaderLength;
		m_headerSize = sizeof(m_fileCount) + sizeof(m_headerSize) + sizeof(m_filePathHeaderLength) + m_filePathHeaderLength;

		for (int i = 0; i < fileCount; ++i)
		{
			m_internalHeaders.push_back(InternalHeader(packedFile));
		}
	}

	TopHeader::TopHeader(uint64_t fileCounnt, const std::vector<astring>& const ignoreFiles)
	{
		m_fileCount = fileCounnt;
		m_ignoreFiles = ignoreFiles;

		m_filePathHeaderLength = 0;
		for (const auto& current : m_ignoreFiles)
		{
			// current + '\t'
			m_filePathHeaderLength += (current.length() + 1u);
		}
		m_headerSize = sizeof(m_fileCount) + sizeof(m_headerSize) + sizeof(m_filePathHeaderLength) + m_filePathHeaderLength;
	}

	void TopHeader::ToByteArray(std::vector<uint8_t>& buffer)
	{

		const auto str = reinterpret_cast<const uint8_t*>(Signature.c_str());
		//FileUtility::SubstitutionRange(buffer, 0, str, Signature.size()*sizeof(achar));

		 
	}
	void TopHeader::GetIgnoreFiles(std::vector<astring>& files)
	{
		files = m_ignoreFiles;
	}

	void TopHeader::AddIgnoreFiles(std::vector<astring>& files)
	{
		auto index = files.size();
		files.resize(index + m_ignoreFiles.size());
		for (const auto& ig : m_ignoreFiles)
			files[index++] = ig;
	}

	const astring TopHeader::Signature = ToAString("pack");
}