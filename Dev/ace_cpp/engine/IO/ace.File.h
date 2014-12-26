#pragma once

#include "../ace.CoreToEngine.h"
#include "../core/IO/ace.File.h"
#include <iterator>
#include <vector>
#include <memory>

namespace ace
{
	class File : public IReference
	{
	private:
		// static ファイルとstream ファイルの参照を持つ
		std::shared_ptr<CoreFile> m_coreFile;

	public:
		File();
		virtual ~File() { }
		void AddRootDirectories(const achar* path);
		void AddRootPackage(const achar* path, const achar* key);
		void ClearRootDirectories();
		//virtual void GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const = 0;
		//virtual void EnumerateFiles(const achar* path) const = 0;
		//virtual void EnumerateFiles(const achar* path, const achar* searchPattern) const = 0;
		//virtual void EnumerateFiles(const achar* path, const achar* searchPattern, bool isRecursive) const = 0;
		bool Exists(const achar* path) const;
		StaticFile* CreateStaticFile(const achar* path);
		StreamFile* CreateStreamFile(const achar* path);
	};
}