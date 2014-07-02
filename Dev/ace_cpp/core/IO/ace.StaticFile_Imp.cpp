#include "ace.StaticFile_Imp.h"

namespace ace
{
	StaticFile_Imp::~StaticFile_Imp()
	{
	}

	StaticFile_Imp::StaticFile_Imp(std::shared_ptr<BaseFile_Imp>& file)
	{
		file->ReadAllBytes(m_buffer);
	}
}
