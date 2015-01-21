#include "ace.StaticFile_Imp.h"

namespace ace
{
	StaticFile_Imp::~StaticFile_Imp()
	{
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile_Imp>& file)
	{
		file->ReadAllBytes(m_buffer);

		m_path = file->FullPath();
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile_Imp>& packedFile, InternalHeader& internalHeader)
	{
		packedFile->Seek(internalHeader.GetOffset());
		packedFile->ReadBytes(m_buffer, internalHeader.GetSize());

		m_path = packedFile->FullPath() + internalHeader.GetFileName();
	}
}
