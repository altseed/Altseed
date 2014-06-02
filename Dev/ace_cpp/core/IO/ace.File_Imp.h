#pragma once

#include "../ace.Core.Base.h"
#include "../Core/ace.Core.Base.h"
#include "ace.BaseFile_Imp.h"
#include "ace.File.h"
#include "ace.BaseFile_Imp.h"
#include "ace.Path_Imp.h"
#include <unordered_map>
#include <memory>
#include <vector>

namespace ace
{
	class BaseFile_Imp;

	struct RootPath_Imp
	{
	public:
		Path_Imp m_path;
		astring m_key;
		bool m_isPackFile;

		RootPath_Imp() : RootPath_Imp(astring(), astring())
		{
		}

		RootPath_Imp(const astring& path, const astring& key) :
			m_path(path),
			m_key(key),
			m_isPackFile(false)
		{
			for (const auto& elem : path)
			{
				// if (IsPack(elem))
				// {
				//    m_isPackFile = true;
				//    break;
				// }
			}
		}
	};

	class File_Imp : public File
	{
	private:
		std::unordered_map<astring, std::unique_ptr<BaseFile_Imp>> m_fileCash;
		std::vector<RootPath_Imp> m_rootPathes;

	public:
		virtual ~File_Imp() { }
		virtual void SetRootDirectories(const astring& path);
		virtual void GetRootDirectories();
		virtual void EnumerateFiles(const astring& path);
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern);
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive);
		virtual bool Exists(const astring& path);
	};
}