#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class MyLayer2DComponent : public Layer2DComponent
{
private:
	int time;
	Layer2D* m_expectedOwner;

public:
	MyLayer2DComponent(Layer2D* expectedOwner)
		: time(0)
		, m_expectedOwner(expectedOwner)
	{
	}

protected:
	void OnUpdate()
	{
		if (time == 0)
		{
			ASSERT_EQ(GetOwner(), m_expectedOwner);
		}

		if (time % 10 == 0)
		{
			auto object = make_shared<TextureObject2D>();
			object->SetPosition(Vector2DF(time % 320, time % 240));
			object->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));
			GetOwner()->AddObject(object);
		}

		++time;
	}
};

class ObjectSystem_LayerComponent : public EngineTest
{
public:
	ObjectSystem_LayerComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayerComponent"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto componentName = ToAString("generator");
		auto component = make_shared<MyLayer2DComponent>(layer.get());

		layer->AddComponent(component, componentName);
		ASSERT_NE(layer->GetComponent(componentName), nullptr);
	}
};

void Test_LayerComponent_GL()
{
	RunTest<ObjectSystem_LayerComponent>(true);
}

TEST(ObjectSystem, LayerComponent_GL)
{
	RunTest<ObjectSystem_LayerComponent>(true);
}

#if _WIN32
TEST(ObjectSystem, LayerComponent_DX)
{
	RunTest<ObjectSystem_LayerComponent>(false);
}
#endif
