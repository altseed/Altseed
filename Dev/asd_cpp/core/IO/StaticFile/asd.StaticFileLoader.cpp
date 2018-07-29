#include "asd.StaticFileLoader.h"
#include "../asd.StaticFile_Imp.h"
#include "../asd.File_Imp.h"

namespace asd
{
	StaticFileLoader::StaticFileLoader(File_Imp* file, StaticFileFactoryBase::Ptr factory,
		StaticFileCacheStore::Ptr cacheStore)
		: file(file)
		, factory(factory)
		, cacheStore(cacheStore)
	{
	}

	StaticFileLoader::~StaticFileLoader()
	{
	}

	StaticFile_Imp* StaticFileLoader::LoadFromAbsolutePath(const char16_t * path)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto cacheKey = astring(path);
		auto cache = cacheStore->Load(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		auto file = CreateSharedPtr(new BaseFile(path));
		if (file->IsValid())
		{
			auto staticFile = factory->LoadFromBaseFile(file, cacheKey);
			cacheStore->Register(cacheKey, staticFile);
			return staticFile;
		}

		return nullptr;
	}

	StaticFile_Imp* StaticFileLoader::LoadFromPackedRoot(const char16_t * path, astring packedPath,
		FileRoot::Ptr root)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto cacheKey = packedPath;
		auto cache = cacheStore->Load(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		auto packFile = root->GetPackFile();

		if (packFile->HaveFile(packedPath))
		{
			const auto& internalHeader = packFile->GetInternalHeader(packedPath);

			auto staticFile = factory->LoadFromPackedFile(packedPath, packFile->RawFile(),
				*internalHeader, root->decryptor);
			cacheStore->Register(cacheKey, staticFile);
			return staticFile;
		}

		return nullptr;
	}

	StaticFile_Imp* StaticFileLoader::LoadFromNonPackedRoot(const char16_t * path, FileRoot::Ptr root)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto combinedPath = FileHelper::CombineRootPath(root->m_path, path);

		auto cacheKey = combinedPath;
		auto cache = cacheStore->Load(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		StaticFile_Imp* staticFile = nullptr;

		auto file = CreateSharedPtr(new BaseFile(combinedPath));
		if (file->IsValid())
		{
			auto staticFile = factory->LoadFromBaseFile(file, cacheKey);
			cacheStore->Register(cacheKey, staticFile);
			return staticFile;
		}

		return nullptr;
	}
}
