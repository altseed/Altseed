#include <gtest/gtest.h>
#include <ace.h>
#include "EngineTest.h"

using namespace ace;

EngineTest::EngineTest(astring title, bool isOpenGLMode, int exitTime)
	: m_isOpenGLMode(isOpenGLMode)
	, m_title(title)
	, m_exitTime(exitTime)
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
			astring filename = ToAString("./TestSS/") + m_title + ToAString(".png");
			engine->TakeScreenshot( filename.c_str() );
		}
		if (time == m_exitTime + 30)
		{
			break;
		}

		++time;
	}

	OnFinish();

	engine->Terminate();

	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_EQ(ref, 0);
}