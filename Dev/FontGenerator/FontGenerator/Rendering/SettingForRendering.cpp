#include "SettingForRendering.h"

using namespace std;

namespace FontGenerator
{
	SettingForRendering::SettingForRendering()
		: m_fontSize(20)
		, m_fontColor(Color())
		, m_border(nullptr)
		, m_bold(nullptr)
	{
	}

	GlyphWithBorder SettingForRendering::ProcessGlyph(Glyph::Ptr source)
	{
		Glyph::Ptr body = source;
		Glyph::Ptr outline = nullptr;

		body->SetColor(m_fontColor);

		if (m_bold != nullptr)
		{
			body = body->Enbolden(m_bold->width);
		}

		if (m_border != nullptr)
		{
			outline = body->Border(m_border->width, m_border->color);
		}

		return GlyphWithBorder(body, outline);
	}

#pragma region GetSet
	int SettingForRendering::GetFontSize() const
	{
		return m_fontSize;
	}

	void SettingForRendering::SetFontSize(int value)
	{
		m_fontSize = value;
	}

	Color SettingForRendering::GetFontColor() const
	{
		return m_fontColor;
	}

	void SettingForRendering::SetFontColor(Color value)
	{
		m_fontColor = value;
	}

	BorderSetting::Ptr SettingForRendering::GetBorder() const
	{
		return m_border;
	}

	void SettingForRendering::SetBorder(BorderSetting::Ptr value)
	{
		m_border = value;
	}

	BoldSetting::Ptr SettingForRendering::GetBold() const
	{
		return m_bold;
	}

	void SettingForRendering::SetBold(BoldSetting::Ptr value)
	{
		m_bold = value;
	}

#pragma endregion
}