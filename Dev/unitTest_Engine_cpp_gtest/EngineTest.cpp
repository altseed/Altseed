#include <gtest/gtest.h>
#include <ace.h>
#include <string>
#include "EngineTest.h"

using namespace std;
using namespace ace;

#if _WIN32
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/stat.h>
#endif

static void CreateSSDirectory()
{
	const char* testDirectory = "ScreenShotTest_Engine_cpp";
#if _WIN32
	if (!PathIsDirectoryA(testDirectory))
	{
		CreateDirectoryA(testDirectory, NULL);
	}
#else
	mkdir(testDirectory,
		S_IRUSR | S_IWUSR | S_IXUSR |
		S_IRGRP | S_IWGRP | S_IXGRP |
		S_IROTH | S_IXOTH | S_IXOTH);
#endif
}


EngineTest::EngineTest(astring title, bool isOpenGLMode, int exitTime)
: m_isOpenGLMode(isOpenGLMode)
, m_title(title)
, m_currentTime(0)
, m_exitTime(exitTime)
, directory(ToAString("./ScreenShotTest_Engine_cpp/"))
{
}

void EngineTest::OnStart()
{
}

void EngineTest::OnUpdating()
{
}

void EngineTest::OnUpdated()
{
}

void EngineTest::OnFinish()
{
}

void EngineTest::Run()
{
	CreateSSDirectory();

	EngineOption option;
	option.GraphicsType = m_isOpenGLMode ? ace::GraphicsType::OpenGL : ace::GraphicsType::DirectX11;
	option.IsMultithreadingMode = true;

	auto initialized = ace::Engine::Initialize(m_title.c_str(), WindowWidth, WindowHeight, option);
	ASSERT_EQ(true, initialized);

	OnStart();

	while (ace::Engine::DoEvents())
	{
		OnUpdating();
		ace::Engine::Update();
		OnUpdated();

		if (m_currentTime == m_exitTime)
		{
			string tail = m_isOpenGLMode ? "_GL" : "_DX";
			tail += ".png";
			astring fileName = directory + m_title + ToAString(tail.c_str());

			ace::Engine::TakeScreenshot(fileName.c_str());
		}
		if (m_currentTime == m_exitTime + 2)
		{
			break;
		}

		++m_currentTime;
	}

	OnFinish();

	ace::Engine::Terminate();
}

void AssertMemoryDoesntLeak()
{
	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_EQ(0, ref);
}
