#pragma once

#include "../ace.Core.Base.h"
#include "ace.StaticFile.h"

namespace ace
{
	class StaticFile_Imp : StaticFile
	{
	private:

	public:
		StaticFile_Imp(BaseFile_Imp& file);
	};
}