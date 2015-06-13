
#include "asd.File_Imp.h"
#include "asd.StaticFile_Imp.h"

namespace asd
{
	StaticFile_Imp::StaticFile_Imp(File_Imp* file, const astring& cacheKey, std::shared_ptr<BaseFile>& baseFile)
		: file(file)
		, cacheKey(cacheKey)
	{
		baseFile->ReadAllBytes(m_buffer);

		m_path = baseFile->GetFullPath();

		SafeAddRef(file);

		isInPackage = false;
	}

	StaticFile_Imp::StaticFile_Imp(File_Imp* file, const astring& cacheKey, const std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader, std::shared_ptr<Decryptor> decryptor)
		: file(file)
		, cacheKey(cacheKey)
	{
		packedFile->Seek(internalHeader.GetOffset());
		packedFile->ReadBytes(m_buffer, internalHeader.GetSize(), decryptor.get(), internalHeader.GetOffset());

		m_path = packedFile->GetFullPath() + ToAString("?") + internalHeader.GetFileName();

		SafeAddRef(file);

		isInPackage = true;
	}

	StaticFile_Imp::~StaticFile_Imp()
	{
		file->UnregisterStaticFile(cacheKey);
		SafeRelease(file);
	}

	bool StaticFile_Imp::GetIsInPackage() const
	{
		return isInPackage;
	}
}
