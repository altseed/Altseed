#include "GlyphWithBorder.h"

namespace FontGenerator
{
	GlyphWithBorder::GlyphWithBorder(Glyph::Ptr& body, Glyph::Ptr outline)
		: m_body(body), m_outline(outline)
	{
	}

	Glyph::Ptr GlyphWithBorder::GetBody() const
	{
		return m_body;
	}

	Glyph::Ptr GlyphWithBorder::GetOutline() const
	{
		return m_outline;
	}

	int GlyphWithBorder::GetAdvance() const
	{
		return m_outline != nullptr ? m_outline->GetAdvance() : m_body->GetAdvance();
	}

	void GlyphWithBorder::Draw(std::int32_t* buffer, int width, int height, int x, int y)
	{
		if (m_outline != nullptr)
		{
			m_outline->Draw(buffer, width, height, x, y);
		}
		m_body->Draw(buffer, width, height, x, y);
	}
}