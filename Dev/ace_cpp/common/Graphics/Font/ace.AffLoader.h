#pragma once
#include <fstream>
#include <vector>
#include <map>
#include "../../ace.common.Base.h"
#include "ace.GlyphData.h"

namespace ace
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