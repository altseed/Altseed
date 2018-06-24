#pragma once
#include "../../asd.Core.Base.h"

#include "../asd.FileRoot.h"
#include "../asd.File.h"
#include "../asd.PackFile.h"
#include "../asd.FileHelper.h"
#include "StaticFileFactoryBase.h"
#include <unordered_map>
#include <mutex>

namespace asd
{
	class StaticFileLoader
	{
	private:
		std::unordered_map<astring, StaticFile_Imp*> staticFiles;
		File_Imp* file;
		StaticFileFactoryBase::Ptr factory;
		std::recursive_mutex		mtx_;

		StaticFile* LoadFromCache(astring cacheKey);

	public:
		typedef std::shared_ptr<StaticFileLoader> Ptr;

		StaticFileLoader(File_Imp* file, StaticFileFactoryBase::Ptr factory);
		virtual ~StaticFileLoader();

		StaticFile* LoadFromAbsolutePath(const char16_t* path);
		StaticFile* LoadFromPackedRoot(const char16_t* path, astring packedPath, FileRoot::Ptr root);
		StaticFile* LoadFromNonPackedRoot(const char16_t* path, FileRoot::Ptr root);

#ifndef SWIG
		void UnregisterStaticFile(const astring& key);
#endif
	};
}