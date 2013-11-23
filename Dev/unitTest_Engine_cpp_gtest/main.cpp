
#include<gtest/gtest.h>

#ifdef _DEBUG
#pragma comment(lib,"Debug/ace_engine.lib")
#pragma comment(lib, "gtestd.lib")
#else
#pragma comment(lib,"Release/ace_engine.lib")
#pragma comment(lib, "gtest.lib")
#endif

/*
#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/ace_common.lib")
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/glfw3.lib")
#pragma comment(lib,"libpng16.Debug.lib")
#pragma comment(lib,"zlib.Debug.lib")
#else
#pragma comment(lib,"Release/ace_common.lib")
#pragma comment(lib,"Release/ace_core.lib")
#pragma comment(lib,"Release/glfw3.lib")
#pragma comment(lib,"libpng16.Release.lib")
#pragma comment(lib,"zlib.Release.lib")
#endif
#pragma comment(lib,"opengl32.lib")

#endif
*/

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

extern void Test_LayerComponent_GL();
extern void Test_Component_GL();
extern void Test_ParentObject();
extern void Graphics_CustomPostEffect(bool isOpenGLMode);
extern void Graphics_PostEffectGrayScale(bool isOpenGLMode);
extern void Graphics_PostEffectSepia(bool isOpenGLMode);
extern void Graphics_PostEffectSmoothing(bool isOpenGLMode);


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

	//Test_ParentObject();
	//auto result = 0;

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