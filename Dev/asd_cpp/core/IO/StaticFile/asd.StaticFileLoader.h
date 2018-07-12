#pragma once
#include "../../asd.Core.Base.h"

#include "../asd.FileRoot.h"
#include "../asd.File.h"
#include "../asd.PackFile.h"
#include "../asd.FileHelper.h"
#include "asd.StaticFileFactoryBase.h"
#include "asd.StaticFileCacheStore.h"
#include <unordered_map>
#include <mutex>

namespace asd
{
	class StaticFileLoader
	{
	private:
		File_Imp* file;
		StaticFileFactoryBase::Ptr factory;
		StaticFileCacheStore::Ptr cacheStore;
		std::recursive_mutex		mtx_;

	public:
		typedef std::shared_ptr<StaticFileLoader> Ptr;

		StaticFileLoader(File_Imp* file, StaticFileFactoryBase::Ptr factory, StaticFileCacheStore::Ptr cacheStore);
		virtual ~StaticFileLoader();

		StaticFile_Imp* LoadFromAbsolutePath(const char16_t* path);
		StaticFile_Imp* LoadFromPackedRoot(const char16_t* path, astring packedPath, FileRoot::Ptr root);
		StaticFile_Imp* LoadFromNonPackedRoot(const char16_t* path, FileRoot::Ptr root);
	};
}