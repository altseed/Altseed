#pragma once

#include "../ace.Core.Base.h"
#include "ace.BaseFile.h"
#include "ace.PackFile.h"

namespace ace
{
	/**
		@brief	ファイルのルートが保存されるクラス
	*/
	struct FileRoot
	{
	private:

	public:
		astring m_path;
		astring m_key;
		
		std::shared_ptr<PackFile>	packFile;

		FileRoot(const astring& path, const astring& key = astring())
			: m_key(key)
		{
			auto baseFile = std::shared_ptr<BaseFile>(new BaseFile(path), [](BaseFile* p){ SafeRelease(p); });

			if (baseFile->IsValid())
			{
				auto pack = std::make_shared<PackFile>();
				if (pack->Load(baseFile, key))
				{
					m_path = path;
					packFile = pack;
					return;
				}
			}

			m_path = FileHelper::ToRootDirectoryPath(path);
		}

		bool IsPackFile()
		{
			return packFile != nullptr;
		}

		std::shared_ptr<PackFile> GetPackFile() { return packFile; }

		bool operator== (const FileRoot& target) const
		{
			return m_key == target.m_key &&
				m_path == target.m_path;
		}
	};
}