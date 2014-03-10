#pragma once
#include <fstream>
#include <map>
#include "../common/ace.common.Base.h"
#include "../common/Utility/ace.BinaryReader.h"
#include "ace.GlyphData.h"

namespace ace
{
	class AffLoader
	{
	private:
		std::shared_ptr<std::istream> m_in;

	public:
		AffLoader(astring fileName);

		std::map<achar, GlyphData> GetGlyphs();
	};
}