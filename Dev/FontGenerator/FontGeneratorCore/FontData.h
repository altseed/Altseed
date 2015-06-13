#pragma once
#include <iostream>
#include <asd.common.Base.h>

namespace FontGenerator
{
	struct FontData
	{
	public:
		wchar_t charactor;
		int x, y, width, height, sheetNumber;

		void WriteOut(std::ostream stream);
	};
}