
#include <iostream>
#include <sstream>
#include <fstream>
#include "PngGenerator.h"
#include "Rendering/ImageBuffer.h"
#include "Utility.h"

#if _DEBUG
#pragma comment(lib,"x86/Debug/freetype.lib")
#pragma comment(lib,"libpng16.lib")
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"asd_common.lib")
#pragma comment(lib,"asd_tool.lib")
#else
#pragma comment(lib,"x86/Release/freetype.lib")
#pragma comment(lib,"libpng16.lib")
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"asd_common.lib")
#pragma comment(lib,"asd_tool.lib")
#endif


using namespace std;
using namespace asd;

namespace FontGenerator
{
	PngGenerator::PngGenerator()
		: m_sheetName(asd::ToAString("font.png"))
		, m_fonts(vector<FontData>())
		, m_setting(SettingForRendering())
	{
	}

	ResultOfGeneratingPng PngGenerator::Generate(astring fontPath, vector<achar>& charactors)
	{
		Font font(fontPath);
		font.SetFontSize(m_setting.GetFontSize());

		ImageBuffer buffer(font, m_sheetSize);
		vector<GlyphData> fontData;

		for (auto& glyph : font.GetGlyphs(charactors))
		{
			auto result = buffer.Draw(glyph, m_setting.GetFontColor(), m_setting.GetBorder());
			fontData.push_back(result);
		}

		auto buffers = buffer.GetBuffers();

		for (size_t i = 0; i < buffers.size(); i++)
		{
			std::ostringstream os;
			os << "_" << i << ".png";
			auto pngPath = GetSheetName() + ToAString(os.str().c_str());
			SavePNGImage(pngPath.c_str(), m_sheetSize, m_sheetSize, buffers[i]->data(), false);
		}

		ResultOfGeneratingPng result;
		result.sheetCount = buffers.size();
		result.fonts = fontData;
		return result;
	}

	void PngGenerator::GeneratePreview(astring fontPath, astring path)
	{
		const int IMAGE_SIZE = 256;

#ifdef _WIN32
		vector<achar> previewChars{ L'e', L'E', L'1', L'‚ ', L'‰i' };
#else
		vector<achar> previewChars{ u'e', u'E', u'1', u'‚ ', u'‰i' };
#endif

		Font font(fontPath);
		font.SetFontSize(m_setting.GetFontSize());

		ImageBuffer buffer(font, IMAGE_SIZE);

		for (auto& glyph : font.GetGlyphs(previewChars))
		{
			buffer.Draw(glyph, m_setting.GetFontColor(), m_setting.GetBorder());
		}

		auto buffers = buffer.GetBuffers();

		SavePNGImage(path.c_str(), IMAGE_SIZE, IMAGE_SIZE, buffers[0]->data(), false);
	}

#pragma region GetSet
	astring PngGenerator::GetSheetName() const
	{
		return m_sheetName;
	}

	void PngGenerator::SetSheetName(astring value)
	{
		m_sheetName = value;
	}

	int PngGenerator::GetSheetSize() const
	{
		return m_sheetSize;
	}

	void PngGenerator::SetSheetSize(int value)
	{
		m_sheetSize = value;
	}

	SettingForRendering PngGenerator::GetSetting() const
	{
		return m_setting;
	}

	void PngGenerator::SetSetting(SettingForRendering value)
	{
		m_setting = value;
	}

#pragma endregion
}