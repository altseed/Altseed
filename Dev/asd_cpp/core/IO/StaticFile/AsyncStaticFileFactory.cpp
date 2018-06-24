#include "AsyncStaticFileFactory.h"
#include "../asd.StaticFile_Imp.h"

namespace asd
{
	AsyncStaticFileFactory::AsyncStaticFileFactory(File_Imp * file, Synchronizer::Ptr sync)
		: file(file)
		, sync(sync)
	{
	}

	StaticFile_Imp * AsyncStaticFileFactory::LoadFromBaseFile(
		BaseFile::Ptr & baseFile,
		const astring & cacheKey)
	{
		return new StaticFile_Imp(file, sync, cacheKey, baseFile);
	}

	StaticFile_Imp * AsyncStaticFileFactory::LoadFromPackedFile(
		const astring & cacheKey,
		const BaseFile::Ptr & packedFile,
		PackFileInternalHeader & internalHeader,
		std::shared_ptr<Decryptor> decryptor)
	{
		return new StaticFile_Imp(file, sync, cacheKey, packedFile, internalHeader, decryptor);
	}
}