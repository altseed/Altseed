#include "EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>
#include <string>

using namespace std;
using namespace asd;

#if _WIN32
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/stat.h>
#endif

static void CreateSSDirectory()
{
	const char *testDirectory = "ScreenShotTest_Engine_cpp";
#if _WIN32
	if (!PathIsDirectoryA(testDirectory))
	{
		CreateDirectoryA(testDirectory, NULL);
	}
#else
	mkdir(testDirectory, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);
#endif
}

EngineTest::EngineTest(astring title, bool isOpenGLMode, int exitTime, int32_t width, int32_t height)
	: m_isOpenGLMode(isOpenGLMode), m_title(title), m_currentTime(0), m_exitTime(exitTime),
	  directory(ToAString("./ScreenShotTest_Engine_cpp/")), WindowWidth(width), WindowHeight(height)
{
#if defined(PERFORMANCE_MODE)
	WindowWidth = 1280;
	WindowHeight = 720;
#endif
}

void EngineTest::OnStart() {}

void EngineTest::OnUpdating() {}

void EngineTest::OnUpdated() {}

void EngineTest::OnFinish() {}

shared_ptr<Texture2D> EngineTest::GetTexture()
{
	return Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
}

void EngineTest::Run()
{
	CreateSSDirectory();

	EngineOption option;
	option.GraphicsDevice = m_isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11;
	option.IsReloadingEnabled = true;

	auto initialized = asd::Engine::Initialize(m_title.c_str(), WindowWidth, WindowHeight, option);
	ASSERT_EQ(true, initialized);

#if defined(PERFORMANCE_MODE)
	asd::Engine::SetTargetFPS(10000);
#endif

	OnStart();

	while (asd::Engine::DoEvents())
	{
#if defined(PERFORMANCE_MODE)
		if (GetTime() % 60 == 0)
		{
			printf("FPS : %f\n", asd::Engine::GetCurrentFPS());
		}
#endif
		OnUpdating();
		asd::Engine::Update();
		OnUpdated();

		if (m_currentTime == m_exitTime)
		{
			string tail = m_isOpenGLMode ? "_GL" : "_DX";
			tail += ".png";
			astring fileName = directory + m_title + ToAString(tail.c_str());

			asd::Engine::TakeScreenshot(fileName.c_str());
		}
		if (m_currentTime == m_exitTime + 2)
		{
			break;
		}

		++m_currentTime;
	}

	OnFinish();

	asd::Engine::Terminate();
}

void AssertMemoryDoesntLeak()
{
	auto ref = asd::GetGlobalReferenceCount();
	ASSERT_EQ(0, ref);
}
