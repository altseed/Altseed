#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

void Profiler_Profiling(bool isOpenGLMode)
{
	int time = 0;
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	{
		auto engine = ace::GetEngine();
	
		engine->Initialize(ace::ToAString("Profiling").c_str(), 640, 480, option);
	
		auto profiler = ace::GetProfiler();

		printf("printfで負荷をかけます\n");
		while (engine->DoEvents())
		{
			profiler->Start(12);
			engine->Update();
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

		engine->Terminate();
	}

	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_TRUE(ref == 0);
}

TEST(Profiler, Profiling_GL)
{
	Profiler_Profiling(true);
}

#if _WIN32
TEST(Profiler, Profiling_DX)
{
	Profiler_Profiling(false);
}
#endif

