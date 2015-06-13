#include "asd.GlyphData.h"

namespace asd
{
	GlyphData::GlyphData()
		: m_charactor(0)
		, m_sheetNum(0)
		, m_src(RectI())
	{
	}

	GlyphData::GlyphData(achar charactor, int sheetNum, RectI src)
		: m_charactor(charactor)
		, m_sheetNum(sheetNum)
		, m_src(src)
	{
	}

	void GlyphData::Push(BinaryWriter& writer)
	{
		writer.Push(m_sheetNum);
		writer.Push(m_src.X);
		writer.Push(m_src.Y);
		writer.Push(m_src.Width);
		writer.Push(m_src.Height);
	}

	GlyphData GlyphData::Get(BinaryReader& reader, achar charactor)
	{
		GlyphData result;

		result.m_sheetNum = reader.Get<int>();
		result.m_src.X = reader.Get<int>();
		result.m_src.Y = reader.Get<int>();
		result.m_src.Width = reader.Get<int>();
		result.m_src.Height = reader.Get<int>();

		result.m_charactor = charactor;

		return result;
	}
}