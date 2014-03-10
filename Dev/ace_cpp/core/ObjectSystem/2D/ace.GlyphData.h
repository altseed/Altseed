#pragma once
#include "../common/ace.common.Base.h"
#include "../common/Math/ace.RectI.h"

namespace ace
{
	struct GlyphData
	{
	private:
		achar m_charactor;
		int m_sheetNum;
		RectI m_src;

	public:
		GlyphData(achar charactor, int sheetNum, RectI src);

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