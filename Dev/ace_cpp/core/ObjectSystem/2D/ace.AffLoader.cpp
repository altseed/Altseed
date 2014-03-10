#include "ace.AffLoader.h"
#define CHARCORDE_MAX 65536

using namespace std;

namespace ace
{
	AffLoader::AffLoader(astring fileName)
		: m_in(make_shared<ifstream>(fileName))
	{
	}

	map<achar, GlyphData> GetGlyphs()
	{
		map<achar, GlyphData> result;
		return result;
	}
}