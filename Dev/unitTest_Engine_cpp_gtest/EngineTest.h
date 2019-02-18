
#pragma once

#include "PCH/asd.UnitTestEngineCpp.PCH.h"

#if _WIN32
#define ENGINE_TEST_DX11(group, name)                                                                                                      \
	TEST(group, name##_DX) { RunTest<group##_##name>(false); }

#else
#define ENGINE_TEST_DX11(group, name)
#endif

#define ENGINE_TEST(group, name)                                                                                                           \
	void Test_##group##_##name(bool openGL) { RunTest<group##_##name>(openGL); }                                                           \
                                                                                                                                           \
	TEST(group, name##_GL) { RunTest<group##_##name>(true); }                                                                              \
	ENGINE_TEST_DX11(group, name)

#define EXTERN_ENGINE_TEST(group, name) extern void Test_##group##_##name(bool openGL);

#define CALL_ENGINE_TEST(group, name, flag) Test_##group##_##name(flag);

//#define	PERFORMANCE_MODE 1

class EngineTest
{
private:
	const asd::astring directory;

	int m_exitTime;
	bool m_isOpenGLMode;
	asd::astring m_title;

protected:
	int32_t WindowWidth;
	int32_t WindowHeight;

	int m_currentTime;

	/**
		@brief
	   オーバーライドして、メインループより前のアサーションや初期化を記述できる。
	*/
	virtual void OnStart();

	/**
		@brief
	   オーバーライドして、メインループ内のアサーションなどを記述できる。
	*/
	virtual void OnUpdating();

	/**
		@brief
	   オーバーライドして、メインループ内のアサーションなどを記述できる。
	*/
	virtual void OnUpdated();

	/**
		@brief
	   オーバーライドして、メインループの後のアサーションや終了処理を記述できる。
	*/
	virtual void OnFinish();

	std::shared_ptr<asd::Texture2D> GetTexture();

public:
	EngineTest(asd::astring title, bool isOpenGLMode, int exitTime, int32_t width = 640, int32_t height = 480);
	void Run();
	int32_t GetExitTime() { return m_exitTime; }
	int32_t GetTime() { return m_currentTime; }
};

extern void AssertMemoryDoesntLeak();

/**
	@brief	EngineTestクラスを継承したテスト クラスによるテストを実行する。
	@param	T
   実行するテストクラスの型。引数に、OpenGLモードかどうかを表す真偽値を１つだけとるコンストラクタが必要である。
	@param	isOpenGLMode	OpenGLモードかどうか。
*/
template <class T> extern void RunTest(bool isOpenGLMode)
{
	T(isOpenGLMode).Run();
	AssertMemoryDoesntLeak();
}

namespace asd
{
static std::vector<int8_t> GetBinaryData(astring filePath)
{
	FILE *fp = nullptr;

#if _WIN32
	_wfopen_s(&fp, (const wchar_t *)filePath.c_str(), L"rb");
	if (fp == nullptr)
		return std::vector<int8_t>();
#else
	fp = fopen(ToUtf8String(filePath.c_str()).c_str(), "rb");
	if (fp == nullptr)
		return std::vector<int8_t>();
#endif

	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::vector<int8_t> data;
	data.resize(size);

	fread(&(data[0]), 1, size, fp);
	fclose(fp);

	return data;
}
} // namespace asd