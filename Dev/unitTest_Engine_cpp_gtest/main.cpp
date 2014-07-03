
#include<gtest/gtest.h>
#include "EngineTest.h"

#ifdef _DEBUG
#pragma comment(lib,"Debug/ace_engine.lib")
#pragma comment(lib, "gtestd.lib")
#else
#pragma comment(lib,"Release/ace_engine.lib")
#pragma comment(lib, "gtest.lib")
#endif

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

extern void Test_ObjectSystem_TrasingCamera(bool openGL);

extern void TestCustomObject();
extern void Test_LayerComponent_GL();

extern void Test_LayersFlag(bool openGL);
extern void TestCamera(bool openGL);

EXTERN_ENGINE_TEST(Graphics, PostEffectGaussianBlur)
EXTERN_ENGINE_TEST(Graphics, PostEffectLightBloom)
EXTERN_ENGINE_TEST(Graphics, PostEffectGrayScale)
EXTERN_ENGINE_TEST(Graphics, PostEffectSepia)
EXTERN_ENGINE_TEST(Graphics, CustomPostEffect)

extern void Graphics_Camera3DPostEffect_(bool isOpenGLMode);

extern void ObjectSystem_Text2DTest_(bool isOpenGLMode);

extern void ObjectSystem_Map2DTest_(bool isGL);

EXTERN_ENGINE_TEST(Graphics, EffectObject3D)
EXTERN_ENGINE_TEST(Graphics, SimpleMesh)
EXTERN_ENGINE_TEST(Graphics, ModelObject3D)

EXTERN_ENGINE_TEST(Graphics, EffectObject2D)

/**
	@brief	単体テストを実行する。
	@note
	現在、単体テスト内でEngineを使用した場合の動作は保証していない。
	テスト内でCoreを直接生成する。
*/
int main(int argc, char **argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif
	::testing::InitGoogleTest(&argc, argv);

	ObjectSystem_Text2DTest_(true);
	//return 0;

	//TestCamera(true);
	//TestCamera(false);
	//return 0;

	/*
	ObjectSystem_Text2DTest_(false);
	return 0;
	*/

	auto result = RUN_ALL_TESTS();

	getchar();
	return result;
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