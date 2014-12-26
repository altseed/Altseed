#pragma once

#include "ace.PackFile.Header_Imp.h"
#include "ace.Path_Imp.h"
#include "ace.BaseFile_Imp.h"
#include <memory>
#include <unordered_map>
#include <cassert>

namespace ace
{
	class PackFile_Imp
	{
	private:
		std::shared_ptr<TopHeader> m_TopHeader;
		std::unordered_map<astring, std::shared_ptr<InternalHeader>> m_InternalHeaderDictionaly;
		std::shared_ptr<BaseFile_Imp> m_RawFile;

	public:
		decltype(m_RawFile) RawFile() { return m_RawFile; }

		PackFile_Imp(std::shared_ptr<BaseFile_Imp> packedFile);
		bool HaveFile(const astring& path);
		std::shared_ptr<TopHeader> GetTopHeader();
		std::shared_ptr<InternalHeader> GetInternalHeader(const astring& path);
		void EnumerateFilesWithoutHiddenAttribute(const Path& directoryPath);
		void EnumerateAllFiles(const Path& directoryPath);
		void Encrypt(const Path& path, const astring& key);
		void Run(const astring& targetPath, const astring& packName, bool consistHiddenFiles, const std::vector<astring> ignorePath, const astring& key);
	};
}