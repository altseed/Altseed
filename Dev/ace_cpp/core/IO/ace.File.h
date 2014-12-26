#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include "ace.StaticFile.h"
#include "ace.StreamFile.h"
#include <iterator>
#include <vector>
#include <memory>

namespace ace
{
	class File : public IReference
	{
	private:
		// static ファイルとstream ファイルの参照を持つ
	public:
		virtual ~File() { }
		virtual void AddRootDirectories(const achar* path) = 0;
		virtual void AddRootPackage(const achar* path, const achar* key) = 0;
		virtual void ClearRootDirectories() = 0;
		//virtual void GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const = 0;
		//virtual void EnumerateFiles(const achar* path) const = 0;
		//virtual void EnumerateFiles(const achar* path, const achar* searchPattern) const = 0;
		//virtual void EnumerateFiles(const achar* path, const achar* searchPattern, bool isRecursive) const = 0;
		virtual bool Exists(const achar* path) const = 0;
		virtual StaticFile* CreateStaticFile(const achar* path) = 0;
		//virtual StreamFile* CreateStreamFile(const achar* path) = 0;
	};
}