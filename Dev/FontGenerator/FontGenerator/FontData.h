#pragma once
#include <iostream>
#include <ace.common.Base.h>

namespace FontGenerator
{
	struct FontData
	{
	public:
		int x, y, width, height, sheetNumber;

		void WriteOut(std::ostream stream);
	};
}