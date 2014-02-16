#pragma once
#include <vector>
#include "FontData.h"

namespace FontGenerator
{
	struct ResultOfGeneratingPng
	{
	public:
		int sheetCount;
		std::vector<FontData> fonts;
	};
}