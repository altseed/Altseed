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
	class File
		: public IReference
	{
	protected:
		virtual StaticFile* CreateStaticFile_(const achar* path) = 0;
		virtual StreamFile* CreateStreamFile_(const achar* path) = 0;

	private:
		// static ファイルとstream ファイルの参照を持つ
	public:
		virtual ~File() { }
		virtual void AddRootDirectory(const achar* path) = 0;
		virtual void AddRootPackage(const achar* path, const achar* key) = 0;
		virtual void ClearRootDirectories() = 0;

		virtual bool Exists(const achar* path) const = 0;

		
#ifndef SWIG
		std::shared_ptr<StaticFile> CreateStaticFile(const achar* path)
		{
			return CreateSharedPtrWithReleaseDLL(CreateStaticFile_(path));
		}

		std::shared_ptr<StreamFile> CreateStreamFile(const achar* path)
		{
			return CreateSharedPtrWithReleaseDLL(CreateStreamFile_(path));
		}
#endif
	};
}