#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_LayersFlag : public EngineTest
{
	class MyObject : public TextureObject2D
	{
	public:
		MyObject(float x, float y)
		{
			auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
			SetTexture(texture);
			SetPosition(Vector2DF(x, y));
			SetCenterPosition(Vector2DF(128, 128));
		}

	protected:
		void OnUpdate()
		{
			SetAngle(GetAngle() + 2);
		}
	};

public:
	ObjectSystem_LayersFlag(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayersFlag"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto notDrawnLayer = make_shared<Layer2D>();
		auto notUpdatedLauer = make_shared<Layer2D>();
		auto object1 = make_shared<ObjectSystem_LayersFlag::MyObject>(100, 128);
		auto object2 = make_shared<ObjectSystem_LayersFlag::MyObject>(300, 128);
		auto object3 = make_shared<ObjectSystem_LayersFlag::MyObject>(500, 128);

		notDrawnLayer->SetIsDrawn(false);
		notUpdatedLauer->SetIsUpdated(false);

		layer->AddObject(object1);
		notDrawnLayer->AddObject(object2);
		notUpdatedLauer->AddObject(object3);
		scene->AddLayer(layer);
		scene->AddLayer(notDrawnLayer);
		scene->AddLayer(notUpdatedLauer);
		ace::Engine::ChangeScene(scene);
	}
};

TEST(ObjectSystem, LayersFlag_GL)
{
	RunTest<ObjectSystem_LayersFlag>(true);
}

#if _WIN32
TEST(ObjectSystem, LayersFlag_DX)
{
	RunTest<ObjectSystem_LayersFlag>(false);
}
#endif

void Test_LayersFlag(bool gl)
{
	RunTest<ObjectSystem_LayersFlag>(true);
}