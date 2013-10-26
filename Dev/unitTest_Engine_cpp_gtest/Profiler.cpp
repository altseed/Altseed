#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

void Profiler_Profiling()
{
	auto engine = ace::GetEngine();

	engine->Initialize(ace::ToAString("Profiling").c_str(), 640, 480, false);

	auto profiler = ace::GetProfiler();

	while (engine->DoEvents())
	{
		profiler->Start(12);
		engine->Update();
		profiler->End(12);

		profiler->Start(17);
		printf( "負荷をかけています\n" );
		profiler->End(17);
	}

	engine->Terminate();
}

TEST(Profiler, Profiling)
{
	Profiler_Profiling();
}