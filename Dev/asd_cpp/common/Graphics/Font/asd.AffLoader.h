#pragma once
#include <fstream>
#include <vector>
#include <map>
#include "../../asd.common.Base.h"
#include "asd.GlyphData.h"

namespace asd
{
	class AffLoader
	{
	private:

		std::map<achar, GlyphData> result;
	public:
		AffLoader(std::vector<uint8_t>&);

		std::map<achar, GlyphData> GetGlyphs();
	};
}