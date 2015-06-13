#pragma once
#include "../../asd.common.Base.h"
#include "../../Math/asd.RectI.h"
#include "../../Utility/asd.BinaryWriter.h"
#include "../../Utility/asd.BinaryReader.h"

namespace asd
{
	struct GlyphData
	{
	private:
		achar m_charactor;
		int m_sheetNum;
		RectI m_src;


	public:
		GlyphData();

		GlyphData(achar charactor, int sheetNum, RectI src);

		void Push(BinaryWriter& writer);

		static GlyphData Get(BinaryReader& reader, achar charactor);

		achar GetCharactor() const
		{
			return m_charactor;
		}
		int GetSheetNum() const
		{
			return m_sheetNum;
		}
		RectI GetSrc() const
		{
			return m_src;
		}
	};
}