#pragma once
#include "StaticFileFactoryBase.h"
#include "../../Utils/asd.Synchronizer.h"

namespace asd
{
	class AsyncStaticFileFactory
		: public StaticFileFactoryBase
	{
	private:
		File_Imp* file;
		Synchronizer::Ptr sync;

	public:
		AsyncStaticFileFactory(File_Imp* file, Synchronizer::Ptr sync);
		virtual ~AsyncStaticFileFactory(){}

		virtual StaticFile_Imp* LoadFromBaseFile(BaseFile::Ptr& baseFile,
			const astring& cacheKey);
		virtual StaticFile_Imp* LoadFromPackedFile(const astring& cacheKey,
			const BaseFile::Ptr& packedFile,
			PackFileInternalHeader& internalHeader,
			std::shared_ptr<Decryptor> decryptor);
	};
}