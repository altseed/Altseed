#pragma once
#include "../common/ace.common.Base.h"
#include "../common/Math/ace.RectI.h"
#include "../common/Utility/ace.BinaryWriter.h"
#include "../common/Utility/ace.BinaryReader.h"

namespace ace
{
	struct GlyphData
	{
	private:
		achar m_charactor;
		int m_sheetNum;
		RectI m_src;

		GlyphData();

	public:
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