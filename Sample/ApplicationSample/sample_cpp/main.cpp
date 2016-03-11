#include "SampleGuide.h"
#include "Browser\SampleBrowser.h"
#define INFO(func, title, description) { func, asd::ToAString(#func), asd::ToAString(title), asd::ToAString(description), true }

#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/Altseed.lib")
#else
#pragma comment(lib,"Release/Altseed.lib")
#endif
#endif

#include <string>

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

extern void ImagePackageUI_Basic();
extern void ImagePackageUI_AlphaBlend();
extern void ImagePackageUI_Component();
extern void Pause_Basic();

extern void CustomPostEffect_Invert();
extern void CustomPostEffect_Mosaic();

extern void MapObject2D_Basic();
extern void MapObject2D_Camera();

extern void Action2D_Camera();

int main(int argc, char** argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	std::vector<SampleInfo> samples =
	{
		INFO(ImagePackageUI_Basic, "", ""),
		INFO(ImagePackageUI_AlphaBlend, "", ""),
		INFO(ImagePackageUI_Component, "", ""),

		INFO(Pause_Basic, "ポーズ", "レイヤーの更新を一時停止するサンプル。"),

		INFO(CustomPostEffect_Invert, "", ""),
		INFO(CustomPostEffect_Mosaic, "", ""),

		INFO(MapObject2D_Basic, "", ""),
		INFO(MapObject2D_Camera, "", ""),

		INFO(Action2D_Camera, "", ""),
	};

	auto browser = SampleBrowser(samples);
	browser.Run();

	return 0;
}

#if _WIN32
static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

void GetDirectoryName(char* dst, char* src)
{
	auto Src = std::string(src);
	int pos = 0;
	int last = 0;
	while (Src.c_str()[pos] != 0)
	{
		dst[pos] = Src.c_str()[pos];

		if (Src.c_str()[pos] == L'\\' || Src.c_str()[pos] == L'/')
		{
			last = pos;
		}

		pos++;
	}

	dst[pos] = 0;
	dst[last] = 0;
}
#endif