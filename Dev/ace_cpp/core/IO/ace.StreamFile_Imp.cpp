#include "ace.StreamFile_Imp.h"

namespace ace
{
	StreamFile_Imp::StreamFile_Imp(std::shared_ptr<BaseFile>& file) 
		: m_file(file)
	{
	}

	void StreamFile_Imp::ReadAllByte(std::vector<uint8_t>& buffer)
	{
		m_file->ReadAllBytes(buffer);
	}
}
