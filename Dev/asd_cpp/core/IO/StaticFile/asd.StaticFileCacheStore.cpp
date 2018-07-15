#include "asd.StaticFileCacheStore.h"

namespace asd
{
	StaticFileCacheStore::StaticFileCacheStore()
	{
	}

	void StaticFileCacheStore::Register(astring cacheKey, StaticFile_Imp * staticFile)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);
		staticFiles[cacheKey] = staticFile;
	}

	void StaticFileCacheStore::Unregister(astring cacheKey)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);
		staticFiles.erase(cacheKey);
	}

	StaticFile_Imp * StaticFileCacheStore::Load(astring cacheKey)
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