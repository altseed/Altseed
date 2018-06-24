#pragma once
#include "StaticFileFactoryBase.h"
#include "../../Utils/asd.Synchronizer.h"

namespace asd
{
	class BlockingStaticFileFactory
		: public StaticFileFactoryBase
	{
	private:
		File_Imp* file;

	public:
		typedef std::shared_ptr<BlockingStaticFileFactory> Ptr;

		BlockingStaticFileFactory(File_Imp* file);
		virtual ~BlockingStaticFileFactory();

		virtual StaticFile_Imp* LoadFromBaseFile(BaseFile::Ptr& baseFile,
			const astring& cacheKey) override;
		virtual StaticFile_Imp* LoadFromPackedFile(const astring& cacheKey,
			const BaseFile::Ptr& packedFile,
			PackFileInternalHeader& internalHeader,
			std::shared_ptr<Decryptor> decryptor) override;
	};
}