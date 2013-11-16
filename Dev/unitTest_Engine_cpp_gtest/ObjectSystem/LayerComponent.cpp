#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class MyComponent : public Layer2DComponent
{
private:
	int time;

public:
	MyComponent()
		: time(0)
	{
	}

protected:
	void OnUpdate()
	{
		if (time % 60 == 0)
		{
			auto object = make_shared<TextureObject2D>();
			object->SetPosition(Vector2DF(time % 640, time % 480));
			GetOwner()->AddObject(object);
		}
	}
};

class ObjectSystem_LayerComponent : public EngineTest
{
public:
	ObjectSystem_LayerComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayerComponent"), isOpenGLMode, 120)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		GetEngine()->ChangeScene(scene);
		scene->AddLayer(layer);

		layer->AddComponent(make_shared<MyComponent>(), ToAString("generator").c_str());
		ASSERT_NE(layer->GetComponent(ToAString("generator").c_str()), nullptr);
	}
};

//*
TEST(ObjectSystem, LayerComponent_GL)
{
	ObjectSystem_LayerComponent(true).Run();
}

#if _WIN32
TEST(ObjectSystem, LayerComponent_DX)
{
	ObjectSystem_LayerComponent(false).Run();
}
#endif
//*/