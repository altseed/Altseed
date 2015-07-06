
#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>

#include "dirent.h"
#include "asd.InstalledFontList.h"

namespace asd {

/**
@brief	ディレクトリのファイル一覧を取得する。
@param	base	ベースディレクトリ
*/
static std::vector<astring> EnumerateFiles(
	const astring &base)
{
	DIR *dir;
	std::string base_ = ToUtf8String(base.c_str());
	if ((dir = opendir(base_.c_str())) == nullptr) {
		return std::vector<astring>();
	}
	std::vector<astring> files;
	struct dirent *ent;
	while ((ent = readdir(dir)) != nullptr) {
		const char *file = ent->d_name;
		files.push_back(ToAString(file));
	}
	closedir(dir);
	return files;
}

/**
@brief	フォントファイルのファイル一覧を取得する。
@param	path	フォントファイルパス
*/
static std::vector<InstalledFontList::Font> EnumerateFonts(
	const astring &path)
{
	std::vector<InstalledFontList::Font> fonts;

	const std::string path_ = ToUtf8String(path.c_str());
	NSString *nsUrl = [NSString stringWithUTF8String:(path_.c_str())];
	if (!nsUrl) return fonts;

	CFURLRef cfUrl = (CFURLRef)[NSURL fileURLWithPath:nsUrl];
	if (!cfUrl) return fonts;

	NSArray *fontDescriptors =
		(NSArray*)CTFontManagerCreateFontDescriptorsFromURL(cfUrl);
	if (!fontDescriptors) return fonts;

	int count = [fontDescriptors count];
	for(int n = 0; n < count; ++n) {
		CTFontDescriptorRef fontDescriptor =
			(CTFontDescriptorRef)[fontDescriptors objectAtIndex:n];
		if (!fontDescriptor) continue;

		NSDictionary *nsDict =
			(NSDictionary*)CTFontDescriptorCopyAttributes(fontDescriptor);
		if (!nsDict) continue;

		NSString *nsFontName =
			(NSString*)[nsDict objectForKey:@"NSFontNameAttribute"];
		if (!nsFontName) continue;

		const char *fontName = [nsFontName UTF8String];
		if (!fontName) continue;

		InstalledFontList::Font font;
		font.Name = ToAString(fontName);
		font.Path = path;
		fonts.push_back(font);
	}
	return fonts;
}

void InstalledFontList::Load_Mac()
{
	std::vector<astring> files;
	// システムフォント
	auto sys   = EnumerateFiles(ToAString("/System/Library/Fonts"));
	// ローカルフォント
	auto local = EnumerateFiles(ToAString("/Library/Fonts"));
	std::copy(sys.begin(),sys.end(),std::back_inserter(files));
	std::copy(local.begin(),local.end(),std::back_inserter(files));

	for (auto path : files) {
		auto fonts_ = EnumerateFonts(path);
		std::copy(fonts_.begin(),fonts_.end(),std::back_inserter(Fonts));
	}
}

} // namespace asd
