#include "asd.BlockingStaticFileFactory.h"
#include "../asd.StaticFile_Imp.h"
#include "../asd.File_Imp.h"

namespace asd
{
	BlockingStaticFileFactory::BlockingStaticFileFactory(File_Imp * file)
		: file(file)
	{
	}

	BlockingStaticFileFactory::~BlockingStaticFileFactory()
	{
	}

	StaticFile_Imp * BlockingStaticFileFactory::LoadFromBaseFile(BaseFile::Ptr & baseFile, const astring & cacheKey)
	{
		return new StaticFile_Imp(file, cacheKey, baseFile);
	}

	StaticFile_Imp * BlockingStaticFileFactory::LoadFromPackedFile(const astring & cacheKey, const BaseFile::Ptr & packedFile, PackFileInternalHeader & internalHeader, std::shared_ptr<Decryptor> decryptor)
	{
		return new StaticFile_Imp(file, cacheKey, packedFile, internalHeader, decryptor);
	}
}