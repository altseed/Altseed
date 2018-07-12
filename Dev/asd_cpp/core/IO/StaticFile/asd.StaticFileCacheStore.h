#pragma once

#include "../asd.StaticFile_Imp.h"
#include <unordered_map>

namespace asd
{
	class StaticFileCacheStore
	{
	private:
		std::unordered_map<astring, StaticFile_Imp*> staticFiles;
		std::recursive_mutex mtx_;

	public:
		typedef std::shared_ptr<StaticFileCacheStore> Ptr;

		StaticFileCacheStore();

		void Register(astring cacheKey, StaticFile_Imp* staticFile);
		void Unregister(astring cacheKey);
		StaticFile_Imp* Load(astring cacheKey);
	};
}