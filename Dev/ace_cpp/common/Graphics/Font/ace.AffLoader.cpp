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
		
		auto table = AffIndexTable::Get(reader);
		auto indexes = table.GetIndexes();
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