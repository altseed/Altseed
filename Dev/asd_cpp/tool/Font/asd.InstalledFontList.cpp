
#include "asd.InstalledFontList.h"

#if !(defined(_CONSOLE_GAME))

#ifdef _WIN32
#include <Shlwapi.h>
#include <filesystem>
#pragma comment(lib, "Shlwapi.lib")
#elif defined __APPLE__
#include "dirent.h"
#endif

#include <ft2build.h>
#include FT_SFNT_NAMES_H

#ifdef _WIN32
#if _MSC_VER > 1900
namespace filesystem = std::filesystem;
#elif _MSC_VER == 1900
namespace filesystem = std::tr2::sys;
#else
namespace filesystem = std::experimental::filesystem;
#endif
#endif

namespace asd
{
/**
@brief	ディレクトリのファイルパス一覧を取得する。
@param	root	[in]	ディレクトリ
@param	paths	[out]	ファイルパス一覧
*/
static void EnumerateFiles(const std::string &root,
						   std::vector<std::string> &paths)
{
#ifdef _WIN32
	filesystem::path fontRoot(root.c_str());
	std::for_each(filesystem::directory_iterator(fontRoot),
				  filesystem::directory_iterator(),
				  [&paths](const filesystem::path &p) {
					  if (filesystem::is_regular_file(p))
					  {
						  auto p_ = p.string();
						  paths.push_back(p_);
					  }
				  });
#elif defined __APPLE__
	DIR *dir;
	if ((dir = opendir(root.c_str())) == nullptr)
	{
		return;
	}
	struct dirent *ent;
	while ((ent = readdir(dir)) != nullptr)
	{
		const char *file = ent->d_name;
		paths.push_back(root + std::string(file));
	}
	closedir(dir);
#endif
}

static std::string GetFileExt(const char *filepath)
{
	auto path = std::string(filepath);
	size_t i = path.rfind('.', path.length());
	if (i != std::string::npos)
	{
		return (path.substr(i + 1, path.length() - i));
	}
	return std::string();
}

static char tolower_(char in)
{
	if (in <= 'Z' && in >= 'A')
	{
		return in - ('Z' - 'z');
	}
	return in;
}

bool InstalledFontList::isLoaded = false;
std::vector<InstalledFontList::Font> InstalledFontList::Fonts =
	std::vector<InstalledFontList::Font>();

void InstalledFontList::Load()
{
	if (isLoaded)
		return;
	isLoaded = true;

	std::vector<std::string> paths;
#ifdef _WIN32
	EnumerateFiles("C:\\Windows\\Fonts\\", paths);
#elif defined __APPLE__
	EnumerateFiles("/System/Library/Fonts/", paths);
	EnumerateFiles("/Library/Fonts/", paths);
#endif

	FT_Library library = nullptr;
	auto error = FT_Init_FreeType(&library);

	for (auto &path : paths)
	{
		auto ext_ = GetFileExt(path.c_str());
		std::string ext(ext_);
		std::transform(ext_.begin(), ext_.end(), ext.begin(), tolower_);

		// fonはアウトラインでないので未対応
		if (ext == std::string("fon"))
		{
			continue;
		}

		FT_Face face = nullptr;
		FT_New_Face(library, path.c_str(), 0, &face);
		if (face == nullptr)
			continue;

		Font font;
		font.Name = ToAString(face->family_name) + ToAString(" ") +
					ToAString(face->style_name);
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
}
} // namespace asd

#endif