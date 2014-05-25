#include "ace.AffLoader.h"
#include <vector>
#include <array>
#include "../../Utility/ace.BinaryReader.h"
#include "ace.AffHeader.h"
#define CHARCODE_MAX 65536

using namespace std;

namespace ace
{
	static array<int16_t, CHARCODE_MAX> GetIndexTable(BinaryReader& reader)
	{
		array<int16_t, CHARCODE_MAX> result;
		for (int i = 0; i < result.size(); ++i)
		{
			result[i] = reader.Get<int16_t>();
		}
		return result;
	}

	AffLoader::AffLoader(astring fileName)
		: m_fileName(fileName)
	{
	}

	map<achar, GlyphData> AffLoader::GetGlyphs()
	{
		BinaryReader reader;
		vector<int8_t> bytes = GetBinaryData(m_fileName);
		reader.ReadIn(bytes.begin(), bytes.end());

		auto header = AffHeader::Get(reader);
		auto indexes = GetIndexTable(reader);
	 	auto fontNum = header.GetFontCount();

		map<achar, GlyphData> result;

		for (int16_t i = 0; i < fontNum; ++i)
		{
			auto charactor = distance(indexes.begin(), find(indexes.begin(), indexes.end(), i));
			result[charactor] = GlyphData::Get(reader, charactor);
		}

		return result;
	}
}