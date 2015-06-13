#pragma once
#include <vector>
#include <Graphics/Font/asd.GlyphData.h>
#include "FontData.h"

namespace FontGenerator
{
	struct ResultOfGeneratingPng
	{
	public:
		int sheetCount;
		std::vector<asd::GlyphData> fonts;
	};
}