
#include "EngineTest.h"

#include <gtest/gtest.h>
#include <ace.h>
#include <memory>

void Profiler_Profiling(bool isOpenGLMode)
{
	int time = 0;
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	{	
		ace::Engine::Initialize(ace::ToAString("Profiling").c_str(), 640, 480, option);
	
		auto profiler = ace::Engine::GetProfiler();

		printf("printfで負荷をかけます\n");
		while (ace::Engine::DoEvents())
		{
			profiler->Start(12);
			ace::Engine::Update();
			profiler->End(12);


			profiler->Start(17);
			printf( "*" );
			profiler->End(17);

			++time;
			if (time == 120)
			{
				break;
			}
		}
		printf("\n");

		ace::Engine::Terminate();
	}
}

TEST(Profiler, Profiling_GL)
{
	Profiler_Profiling(true);
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Profiler, Profiling_DX)
{
	Profiler_Profiling(false);
	AssertMemoryDoesntLeak();
}
#endif

