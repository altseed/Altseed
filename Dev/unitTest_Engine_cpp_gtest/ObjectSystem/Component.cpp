#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class MyObject2DComponent : public ace::Object2DComponent
{
private:
	int time;

public:
	MyObject2DComponent()
		: time(0)
	{
	}

protected:
	void OnUpdate()
	{
		if (time % 30 == 0)
		{
			GetOwner()->SetAngle(GetOwner()->GetAngle() + 13);
		}
		++time;
	}
};

class ObjectSystem_Component : public EngineTest
{
public:
	ObjectSystem_Component(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Component"), isOpenGLMode, 120)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();

		GetEngine()->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		object->SetPosition(Vector2DF(320, 240));
		object->AddComponent(make_shared<MyObject2DComponent>(), ToAString("rotation"));
		ASSERT_NE(object->GetComponent(ToAString("rotation").c_str()), nullptr);

		auto g = ace::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(g);
	}
};

void Test_Component_GL()
{
	ObjectSystem_Component(true).Run();
	AssertMemoryDoesntLeak();
}

TEST(ObjectSystem, Component_GL)
{
	ObjectSystem_Component(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(ObjectSystem, Component_DX)
{
	ObjectSystem_Component(false).Run();
	AssertMemoryDoesntLeak();
}
#endif