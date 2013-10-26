/*
#include <ace.h>
#include <gtest/gtest.h>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

void Profiler_Profiling()
{
	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("profiling").c_str(), 640, 480, false);

	auto profiler = ace::GetProfiler();

	while (engine->DoEvents())
	{
		profiler->Start(1);
		Sleep(10);
		profiler->End(1);
		engine->Update();
	}

	engine->Terminate();
}

TEST(Profiler, Profiling)
{
	Profiler_Profiling();
}
*/