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
		virtual void SetRootDirectories(const astring& path) = 0;
		virtual void SetRootDirectories(const astring& path, const astring& path2) = 0;
		virtual void SetRootDirectories(const astring& path, const astring& path2, const astring& path3) = 0;
		virtual void GetRootDirectories(std::vector<std::reference_wrapper<Path>>& rootPathes) const = 0;
		virtual void EnumerateFiles(const astring& path) const = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern) const = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive) const = 0;
		virtual bool Exists(const astring& path) const = 0;
		virtual StaticFile* GetStaticFile(const astring& path) = 0;
		virtual StreamFile* GetStreamFile(const astring& path) = 0;
	};
}