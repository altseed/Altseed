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

		if (time % 60 == 0)
		{
			auto object = make_shared<TextureObject2D>();
			object->SetPosition(Vector2DF(time % 320, time % 240));
			object->SetTexture(GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));
			GetOwner()->AddObject(object);
		}

		++time;
	}
};

class ObjectSystem_LayerComponent : public EngineTest
{
public:
	ObjectSystem_LayerComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayerComponent"), isOpenGLMode, 240)
		, m_layer(nullptr)
	{
	}

protected:
	shared_ptr<Layer2D> m_layer;

	void OnStart()
	{
		auto scene = make_shared<Scene>();
		m_layer = make_shared<Layer2D>();

		GetEngine()->ChangeScene(scene);
		scene->AddLayer(m_layer);

		auto componentName = ToAString("generator");

		m_layer->AddComponent(make_shared<MyLayer2DComponent>(m_layer.get()), componentName);
		ASSERT_NE(m_layer->GetComponent(componentName), nullptr);
	}
};

void Test_LayerComponent_GL()
{
	ObjectSystem_LayerComponent(true).Run();
}

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
