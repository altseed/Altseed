#include "ace.FileUtility.h"
#include "ace.PackFile.Header_Imp.h"
#include "../common/ace.common.Base.h"

namespace ace
{
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