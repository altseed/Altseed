#pragma once
#include <iostream>
#include <asd.common.Base.h>

namespace FontGenerator
{
	struct FontData
	{
	public:
		asd::achar charactor;
		int x, y, width, height, sheetNumber;

		void WriteOut(std::ostream stream);
	};
}