
#include "EngineTest.h"

#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace asd;

class Profiler_Profiling : public EngineTest
{
public:
	Profiler_Profiling(bool isOpenGLMode) : EngineTest(u"Profiling", isOpenGLMode, 60) {}

private:
	asd::Profiler *m_profiler;

protected:
	void OnStart()
	{
		m_profiler = asd::Engine::GetProfiler();
		printf("printfで負荷をかけます\n");

		auto scene = make_shared<Scene>();
		auto layer1 = make_shared<Layer2D>();
		auto layer2 = make_shared<Layer2D>();

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(u"Data/Texture/Cloud1.png");
		for (size_t i = 0; i < 100; i++)
		{
			auto obj = make_shared<TextureObject2D>();
			obj->SetTexture(texture);
			layer1->AddObject(obj);
		}

		layer1->SetName(ToAString("Test1"));
		layer2->SetName(ToAString("Test2"));

		Engine::ChangeScene(scene);
		scene->AddLayer(layer1);
		scene->AddLayer(layer2);
	}

	void OnUpdating()
	{
		if (m_currentTime == 20)
		{
			asd::Engine::SetProfilerVisibility(true);
		}

		m_profiler->Start(17);
		printf("*");
		m_profiler->End(17);

		m_profiler->Start(9);
		printf("*");
		printf("*");
		m_profiler->End(9);

		m_profiler->Start(12);
	}

	void OnUpdated() { m_profiler->End(12); }
};

ENGINE_TEST(Profiler, Profiling)
