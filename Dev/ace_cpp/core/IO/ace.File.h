#pragma once

#include "../ace.Core.Base.h"
#include "ace.StaticFile.h"
#include "ace.StreamFile.h"

namespace ace
{
	class File
	{
	private:
		// static ファイルとstream ファイルの参照を持つ
	public:
		virtual ~File() { }
		virtual void SetRootDirectories(const astring& path) = 0;
		virtual void GetRootDirectories() = 0;
		virtual void EnumerateFiles(const astring& path) = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern) = 0;
		virtual void EnumerateFiles(const astring& path, const astring& searchPattern, bool isRecursive) = 0;
		virtual bool Exists(const astring& path) = 0;
	};
}