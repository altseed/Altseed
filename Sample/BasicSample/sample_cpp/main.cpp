#include "SampleGuide.h"
#define INFO(func) { func, #func }

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

extern void Basic_Empty();

extern void Geometry2D_ArcShape();
extern void Geometry2D_PolygonShape();
extern void Geometry2D_RectangleShape();

extern void PostEffect_GrayScale();
extern void PostEffect_Sepia();
extern void PostEffect_GaussianBlur();
extern void PostEffect_LightBloom();

extern void Keyboard();
extern void Mouse();
extern void Joystick();
extern void Sound();

extern void EffectObject2D();

extern void TextObject2D_Simple();
extern void TextObject2D_Rotate();
extern void TextObject2D_Filtered();

extern void TextureObject2D_Simple();
extern void TextureObject2D_Transform();
extern void TextureObject2D_Turn();
extern void TextureObject2D_Src();

extern void MapObject2D_Simple();
extern void MapObject2D_Camera();

extern void CameraObject2D();

extern void GeometryObject2D_ArcShape();
extern void GeometryObject2D_RectangleShape();
extern void GeometryObject2D_PolygonShape();

extern void Collision2D();
extern void ImagePackage();

extern void Object2DComponent();
extern void Transition();

extern void PostEffect();

int main(int argc, char** argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	std::vector<SampleInfo> samples =
	{
		INFO(Basic_Empty),

		INFO(GeometryObject2D_ArcShape),
		INFO(GeometryObject2D_PolygonShape),
		INFO(GeometryObject2D_RectangleShape),

		INFO(PostEffect_GrayScale),
		INFO(PostEffect_Sepia),
		INFO(PostEffect_GaussianBlur),
		INFO(PostEffect_LightBloom),

		INFO(Keyboard),
		INFO(Mouse),
		INFO(Joystick),

		INFO(Sound),

		INFO(Object2DComponent),
		INFO(Transition),

		INFO(CameraObject2D),
		INFO(EffectObject2D),

		INFO(TextObject2D_Simple),
		INFO(TextObject2D_Rotate),
		INFO(TextObject2D_Filtered),

		INFO(TextureObject2D_Simple),
		INFO(TextureObject2D_Turn),
		INFO(TextureObject2D_Transform),
		INFO(TextureObject2D_Src),

		INFO(MapObject2D_Simple),
		INFO(MapObject2D_Camera),

		INFO(ImagePackage),
		INFO(GeometryObject2D_ArcShape),
		INFO(GeometryObject2D_RectangleShape),
		INFO(GeometryObject2D_PolygonShape),
		INFO(Collision2D),

	};

	auto cui = SampleGuide(samples);
	cui.Show();

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