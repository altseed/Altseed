#include "StaticFileLoader.h"
#include "../asd.StaticFile_Imp.h"
#include "../asd.File_Imp.h"

namespace asd
{
	StaticFileLoader::StaticFileLoader(File_Imp* file, StaticFileFactoryBase::Ptr factory)
		: file(file)
		, factory(factory)
	{
	}

	StaticFileLoader::~StaticFileLoader()
	{
	}

	StaticFile* StaticFileLoader::LoadFromAbsolutePath(const char16_t * path)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto cacheKey = astring(path);
		auto cache = LoadFromCache(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		auto file = CreateSharedPtr(new BaseFile(path));
		if (file->IsValid())
		{
			auto staticFile = factory->LoadFromBaseFile(file, cacheKey);
			staticFiles[cacheKey] = staticFile;
			return staticFile;
		}

		return nullptr;
	}

	StaticFile* StaticFileLoader::LoadFromPackedRoot(const char16_t * path, astring packedPath,
		FileRoot::Ptr root)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto cacheKey = packedPath;
		auto cache = LoadFromCache(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		auto packFile = root->GetPackFile();

		if (packFile->HaveFile(packedPath))
		{
			const auto& internalHeader = packFile->GetInternalHeader(packedPath);

			StaticFile_Imp* staticFile = nullptr;

			{
				staticFile = factory->LoadFromPackedFile(astring(packedPath), packFile->RawFile(),
					*internalHeader, root->decryptor);
				staticFiles[cacheKey] = staticFile;
				return staticFile;
			}
		}
	}

	StaticFile* StaticFileLoader::LoadFromNonPackedRoot(const char16_t * path, FileRoot::Ptr root)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		auto combinedPath = FileHelper::CombineRootPath(root->m_path, path);

		auto cacheKey = combinedPath;
		auto cache = LoadFromCache(cacheKey);
		if (cache != nullptr)
		{
			return cache;
		}

		StaticFile_Imp* staticFile = nullptr;

		auto file = CreateSharedPtr(new BaseFile(combinedPath));
		if (file->IsValid())
		{
			auto staticFile = factory->LoadFromBaseFile(file, cacheKey);
			staticFiles[cacheKey] = staticFile;
			return staticFile;
		}

		return nullptr;
	}

	void StaticFileLoader::UnregisterStaticFile(const astring & key)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		staticFiles.erase(key);
	}

	StaticFile* StaticFileLoader::LoadFromCache(astring cacheKey)
	{
		auto it = staticFiles.find(cacheKey);
		if (it != staticFiles.end())
		{
			auto ret = it->second;
			SafeAddRef(ret);
			return ret;
		}
		return nullptr;
	}
}
