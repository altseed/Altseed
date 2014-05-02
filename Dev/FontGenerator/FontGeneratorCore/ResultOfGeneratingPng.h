#pragma once
#include <vector>
#include <Graphics/Font/ace.GlyphData.h>
#include "FontData.h"

namespace FontGenerator
{
	struct ResultOfGeneratingPng
	{
	public:
		int sheetCount;
		std::vector<ace::GlyphData> fonts;
	};
}