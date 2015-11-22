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

extern void PostEffect_GrayScale();
extern void PostEffect_Sepia();
extern void PostEffect_GaussianBlur();
extern void PostEffect_LightBloom();

extern void Transition_Fade();

extern void Keyboard_Basic();
extern void Keyboard_SingularKey();
extern void Keyboard_PuralKeys();

extern void Mouse_Position();
extern void Mouse_Click();
extern void Mouse_State();
extern void Mouse_Wheel();

extern void Joystick_Basic();
extern void Joystick_Axis();

extern void Sound_SE();
extern void Sound_BGM();
extern void Sound_Loop();

extern void EffectObject2D_Basic();

extern void TextObject2D_Basic();
extern void TextObject2D_DynamicFont();
extern void TextObject2D_Rotate();
extern void TextObject2D_Filtered();
extern void TextObject2D_Centering();

extern void TextureObject2D_Basic();
extern void TextureObject2D_Transform();
extern void TextureObject2D_Turn();
extern void TextureObject2D_Src();

extern void CameraObject2D_Basic();
extern void CameraObject2D_Magnify();
extern void CameraObject2D_Group();
extern void CameraObject2D_Texture();

extern void GeometryObject2D_CircleShape();
extern void GeometryObject2D_ArcShape();
extern void GeometryObject2D_RectangleShape();
extern void GeometryObject2D_PolygonShape();
extern void GeometryObject2D_PolygonShape_Textured();

extern void Collision2D_Basic();
extern void Collision2D_Geometries();

extern void Object2DComponent_Custom();

extern void SceneAndLayer_Basic();
extern void SceneAndLayer_CustomLayer();
extern void SceneAndLayer_CustomScene();

extern void Dispose_Object2D();
extern void Dispose_Layer2D();

extern void File_PackFile();
extern void File_RootDirectory();
extern void File_StaticFile();
extern void File_StreamFile();

extern void EngineMisc_ShowFps();

extern void Texture_Basic();
extern void Texture_Edit();

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
		INFO(TextureObject2D_Basic),
		INFO(TextureObject2D_Turn),
		INFO(TextureObject2D_Src),
		INFO(TextureObject2D_Transform),

		INFO(Transition_Fade),

		INFO(Mouse_Position),
		INFO(Mouse_Click),
		INFO(Mouse_State),
		INFO(Mouse_Wheel),

		INFO(Keyboard_Basic),
		INFO(Keyboard_SingularKey),
		INFO(Keyboard_PuralKeys),

		INFO(Joystick_Basic),
		INFO(Joystick_Axis),

		INFO(Sound_SE),
		INFO(Sound_BGM),
		INFO(Sound_Loop),

		INFO(Object2DComponent_Custom),

		INFO(CameraObject2D_Basic),
		INFO(CameraObject2D_Magnify),
		INFO(CameraObject2D_Group),
		INFO(CameraObject2D_Texture),

		INFO(EffectObject2D_Basic),

		INFO(TextObject2D_Basic),
		INFO(TextObject2D_DynamicFont),
		INFO(TextObject2D_Rotate),
		INFO(TextObject2D_Filtered),
		INFO(TextObject2D_Centering),

		INFO(GeometryObject2D_CircleShape),
		INFO(GeometryObject2D_ArcShape),
		INFO(GeometryObject2D_RectangleShape),
		INFO(GeometryObject2D_PolygonShape),
		INFO(GeometryObject2D_PolygonShape_Textured),

		INFO(Collision2D_Basic),
		INFO(Collision2D_Geometries),

		INFO(SceneAndLayer_Basic),
		INFO(SceneAndLayer_CustomLayer),
		INFO(SceneAndLayer_CustomScene),

		INFO(Dispose_Object2D),
		INFO(Dispose_Layer2D),

		INFO(PostEffect_GrayScale),
		INFO(PostEffect_Sepia),
		INFO(PostEffect_GaussianBlur),
		INFO(PostEffect_LightBloom),

		INFO(File_PackFile),
		INFO(File_RootDirectory),
		INFO(File_StaticFile),
		INFO(File_StreamFile),

		INFO(EngineMisc_ShowFps),

		INFO(Texture_Basic),
		INFO(Texture_Edit),
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