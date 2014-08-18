
#include "DLL.h"

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

		HKEY fontKey;

		// レジストリを開く
		auto r = RegOpenKeyExA(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
			0,
			KEY_READ,
			&fontKey);

		if (r != ERROR_SUCCESS) return;

		for (auto ind = 0;; ind++)
		{
			char valueName[1000];
			DWORD valueNameSize = 1000;

			char value[1000];
			DWORD valueSize = 1000;

			DWORD valueType;

			auto result = SHEnumValueA(
				fontKey,
				ind,
				valueName,
				&valueNameSize,
				&valueType,
				value,
				&valueSize);

			if (result != ERROR_SUCCESS) break;

			fontNames.push_back(valueName);
			fontPathes.push_back(value);
		}

		// 終了
		RegCloseKey(fontKey);
	}

	int32_t DLL::GetFontCount()
	{
		return fontNames.size();
	}

	const char* DLL::GetFontName(int32_t index)
	{
		return fontNames[index].c_str();
	}

	const char* DLL::GetFontPath(int32_t index)
	{
		return fontPathes[index].c_str();
	}


	bool DLL::Run()
	{
		Generator gen;

		try
		{
			SettingForRendering setting;
			setting.SetFontSize(m_fontSize);
			setting.SetFontColor(m_fontColor);

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