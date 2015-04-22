#include "ace.StaticFile_Imp.h"

namespace ace
{
	StaticFile_Imp::~StaticFile_Imp()
	{
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile>& file)
	{
		file->ReadAllBytes(m_buffer);

		m_path = file->GetFullPath();
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile>& packedFile, PackFileInternalHeader& internalHeader)
	{
		packedFile->Seek(internalHeader.GetOffset());
		packedFile->ReadBytes(m_buffer, internalHeader.GetSize());

		m_path = packedFile->GetFullPath() + internalHeader.GetFileName();
	}
}
