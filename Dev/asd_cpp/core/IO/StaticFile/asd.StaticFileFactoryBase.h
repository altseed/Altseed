#pragma once
#include "../../asd.Core.Base.h"

#include "../asd.FileRoot.h"
#include "../asd.File.h"
#include <memory>

namespace asd
{
	class StaticFileFactoryBase
	{
	public:
		typedef std::shared_ptr<StaticFileFactoryBase> Ptr;

		virtual ~StaticFileFactoryBase(){}

		virtual StaticFile_Imp* LoadFromBaseFile(BaseFile::Ptr& baseFile, const astring& cacheKey) = 0;
		virtual StaticFile_Imp* LoadFromPackedFile(const astring& cacheKey, const BaseFile::Ptr& packedFile,
			PackFileInternalHeader& internalHeader, std::shared_ptr<Decryptor> decryptor) = 0;
	};
}