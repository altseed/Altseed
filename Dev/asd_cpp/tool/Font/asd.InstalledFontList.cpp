#include "asd.InstalledFontList.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#endif

namespace asd
{
	bool InstalledFontList::isLoaded = false;
	std::vector<InstalledFontList::Font> InstalledFontList::Fonts = std::vector<InstalledFontList::Font>();

	void InstalledFontList::Load()
	{
		if (isLoaded) return;
		isLoaded = true;

#ifdef _WIN32
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

			Font font;
			font.Name = ToAString(valueName);
			font.Path = ToAString(value);

			Fonts.push_back(font);
		}

		// 終了
		RegCloseKey(fontKey);
#elif defined __APPLE__
		Load_Mac();
#endif
	}
}