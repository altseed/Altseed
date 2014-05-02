
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
			setting.SetBold(std::make_shared<BoldSetting>(1));
			setting.SetBorder(std::make_shared<BorderSetting>(4, Color(0, 0, 0, 255)));

			gen.GenerateFontFile(
				m_fontName.c_str(),
				m_textFilePath.c_str(),
				m_exportPath.c_str(),
				setting);
		}
		catch (const char* err)
		{
			std::cout << err << std::endl;
		}

		return true;
	}
}