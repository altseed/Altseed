#include "ace.AffLoader.h"
#include <vector>
#include <array>
#include "../../Utility/ace.BinaryReader.h"
#include "ace.AffHeader.h"
#include "ace.AffIndexTable.h"
#define CHARCODE_MAX 65536

using namespace std;

namespace ace
{
	AffLoader::AffLoader(std::vector<uint8_t> &data)
	{
		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		auto header = AffHeader::Get(reader);

		auto table = AffIndexTable::Get(reader);
		auto indexes = table.GetIndexes();
		auto fontNum = header.GetFontCount();

		for (int16_t i = 0; i < fontNum; ++i)
		{
			auto charactor = distance(indexes.begin(), find(indexes.begin(), indexes.end(), i));
			result[charactor] = GlyphData::Get(reader, charactor);
		}
	}

	map<achar, GlyphData> AffLoader::GetGlyphs()
	{

		return result;
	}
}