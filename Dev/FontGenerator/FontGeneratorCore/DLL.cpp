
#include "DLL.h"

namespace FontGenerator
{
	DLL::DLL()
	{
	}

	DLL::~DLL()
	{
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