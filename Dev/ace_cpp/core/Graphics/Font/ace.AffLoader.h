#pragma once
#include <fstream>
#include <map>
#include "../common/ace.common.Base.h"
#include "ace.GlyphData.h"

namespace ace
{
	class AffLoader
	{
	private:
		astring m_fileName;

	public:
		AffLoader(astring fileName);

		std::map<achar, GlyphData> GetGlyphs();
	};
}