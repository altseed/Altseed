
#include "DLL.h"

#ifdef _WIN32
#pragma comment(lib,"Shlwapi.lib")
#else
#include <stdlib.h>
#endif

namespace FontGenerator
{
	DLL::DLL()
	{
		LoadFontNames();
	}

	DLL::~DLL()
	{
	}

	void DLL::LoadFontNames()
	{
		fontNames.clear();
		fontPathes.clear();

		asd::InstalledFontList::Load();

		for (auto& font : asd::InstalledFontList::Fonts)
		{
			fontNames.push_back(font.Name);
			fontPathes.push_back(font.Path);
		}
	}

	int32_t DLL::GetFontCount()
	{
		return fontNames.size();
	}

	const asd::achar* DLL::GetFontName(int32_t index)
	{
		return fontNames[index].c_str();
	}

	const asd::achar* DLL::GetFontPath(int32_t index)
	{
		return fontPathes[index].c_str();
	}

	const asd::achar* DLL::SavePreview()
	{
#ifdef _WIN32
		char tempPath[260];
		char dirPath[256];
		GetTempPathA(256, dirPath);

		GetTempFileNameA(dirPath, "aff", 0, tempPath);
#else
		char tempPath[] = "/tmp/aff_preview_XXXXXX";
		mkstemp(tempPath);
#endif

		PngGenerator png;
		SettingForRendering setting;
		setting.SetFontSize(m_fontSize);
		setting.SetFontColor(m_fontColor);

		if (m_outlineSize > 0)
		{
			setting.SetBorder(std::make_shared<BorderSetting>(m_outlineSize, m_outlineColor, m_outlineSampling));
		}

		static asd::astring atempPath = asd::ToAString(tempPath);

		png.SetSetting(setting);
		png.GeneratePreview(m_fontName, atempPath.c_str());

		return atempPath.c_str();
	}


	bool DLL::Run()
	{
		Generator gen;

		try
		{
			SettingForRendering setting;
			setting.SetFontSize(m_fontSize);
			setting.SetFontColor(m_fontColor);

			if (m_outlineSize > 0)
			{
				setting.SetBorder(std::make_shared<BorderSetting>(m_outlineSize, m_outlineColor, m_outlineSampling));
			}

			gen.SetSetting(setting);
			gen.SetSheetName(m_exportPath);
			gen.SetSheetSize(m_sheetSize);

			gen.GenerateFontFile(
				m_fontName.c_str(),
				m_textFilePath.c_str());
		}
		catch (const char* err)
		{
			std::cout << err << std::endl;
		}

		return true;
	}
}