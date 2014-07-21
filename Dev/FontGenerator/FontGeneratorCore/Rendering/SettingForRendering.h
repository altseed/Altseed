#pragma once
#include <memory>
#include <vector>
#include "Setting.h"
#include "Glyph.h"
#include "GlyphWithBorder.h"

namespace FontGenerator
{
	class SettingForRendering
	{
	private:
		int m_fontSize;
		Color m_fontColor;
		BorderSetting::Ptr m_border;
		BoldSetting::Ptr m_bold;

	public:
		typedef std::shared_ptr<SettingForRendering> Ptr;

		SettingForRendering();

		void ProcessGlyph(Glyph::Ptr source);

		int GetFontSize() const;
		void SetFontSize(int value);

		Color GetFontColor() const;
		void SetFontColor(Color value);

		BorderSetting::Ptr GetBorder() const;
		void SetBorder(BorderSetting::Ptr value);
		
		BoldSetting::Ptr GetBold() const;
		void SetBold(BoldSetting::Ptr value);
	};
}