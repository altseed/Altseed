#include <gtest/gtest.h>
#include <ace.h>
#include <string>
#include "EngineTest.h"

using namespace std;
using namespace ace;

EngineTest::EngineTest(astring title, bool isOpenGLMode, int exitTime)
: m_isOpenGLMode(isOpenGLMode)
, m_title(title)
, m_exitTime(exitTime)
, directory(ToAString("./TestSS/"))
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
	int time = 0;
	EngineOption option;
	option.GraphicsType = m_isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	ASSERT_NE(engine, nullptr);
	engine->Initialize(m_title.c_str(), 640, 480, option);

	OnStart();

	while (engine->DoEvents())
	{
		OnUpdating();
		engine->Update();
		OnUpdated();

		if (time == m_exitTime)
		{
			string tail = m_isOpenGLMode ? "_GL" : "_DX";
			tail += ".png";
			astring fileName = directory + m_title + ToAString(tail.c_str());

			engine->TakeScreenshot(fileName.c_str());
		}
		if (time == m_exitTime + 30)
		{
			break;
		}

		++time;
	}

	OnFinish();

	engine->Terminate();
}

void AssertMemoryDoesntLeak()
{
	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_EQ(0, ref);
}
