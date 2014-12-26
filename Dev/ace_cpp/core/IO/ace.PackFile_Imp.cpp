#include "ace.PackFile_Imp.h"

namespace ace
{
	PackFile_Imp::PackFile_Imp(const astring& path)
	{

	}

	bool PackFile_Imp::HaveFile(const astring& path)
	{
		return m_InternalHeaderDictionaly.find(path) != m_InternalHeaderDictionaly.end();
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