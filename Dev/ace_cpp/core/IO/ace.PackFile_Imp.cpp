#include "ace.PackFile_Imp.h"
#include <vector>
#include <cassert>

namespace ace
{
	PackFile_Imp::PackFile_Imp(std::shared_ptr<BaseFile_Imp> packedFile)
	{
		m_TopHeader = std::make_shared<TopHeader>(packedFile);
		auto internalHeaders = m_TopHeader->GetInternalHeaders();

		for (auto header : internalHeaders)
		{
			m_InternalHeaderDictionaly.emplace(header.GetFileName(), std::shared_ptr<InternalHeader>(&header));
		}

		m_RawFile = packedFile;
	}

	bool PackFile_Imp::HaveFile(const astring& path)
	{
		astring tmp(path);
		std::transform(path.begin(), path.end(), tmp.begin(), ::towlower);
		return m_InternalHeaderDictionaly.find(tmp) != m_InternalHeaderDictionaly.end();
	}
	std::shared_ptr<TopHeader> PackFile_Imp::GetTopHeader()
	{
		return m_TopHeader;
	}
	std::shared_ptr<InternalHeader> PackFile_Imp::GetInternalHeader(const astring& path)
	{
		if (HaveFile(path))
			return m_InternalHeaderDictionaly[path];
		else
			assert(false);
	}
	void PackFile_Imp::EnumerateFilesWithoutHiddenAttribute(const Path& directoryPath)
	{
	}
	void PackFile_Imp::EnumerateAllFiles(const Path& directoryPath)
	{
	}
	void PackFile_Imp::Encrypt(const Path& path, const astring& key)
	{
	}
	void PackFile_Imp::Run(const astring& targetPath, const astring& packName, bool consistHiddenFiles, const std::vector<astring> ignorePath, const astring& key)
	{
	}
}