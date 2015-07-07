#include "asd.InstalledFontList.h"

#ifdef _WIN32
#include <filesystem>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#endif

#include <ft2build.h>
#include FT_SFNT_NAMES_H

namespace asd
{
	bool InstalledFontList::isLoaded = false;
	std::vector<InstalledFontList::Font> InstalledFontList::Fonts = std::vector<InstalledFontList::Font>();

	void InstalledFontList::Load()
	{
		if (isLoaded) return;
		isLoaded = true;

#ifdef _WIN32

		std::vector<std::string> paths;
		std::tr2::sys::path fontRoot("C:\\Windows\\Fonts\\");
		
		std::for_each(
			std::tr2::sys::directory_iterator(fontRoot), 
			std::tr2::sys::directory_iterator(),
			[&paths](const std::tr2::sys::path& p) {
			if (std::tr2::sys::is_regular_file(p))
			{
				paths.push_back(p);
			}});

		FT_Library library = nullptr;
		auto error = FT_Init_FreeType(&library);

		for (auto& path : paths)
		{
			FT_Face face = nullptr;
			FT_New_Face(library, path.c_str(), 0, &face);
			if (face == nullptr) continue;

			Font font;
			font.Name = ToAString(face->family_name);
			font.Path = ToAString(path.c_str());

			Fonts.push_back(font);

			FT_Done_Face(face);
		}

		FT_Done_FreeType(library);

		// レジストリだとフルネームが長い
		/*
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
		*/

#elif defined __APPLE__
		Load_Mac();
#endif
	}
}