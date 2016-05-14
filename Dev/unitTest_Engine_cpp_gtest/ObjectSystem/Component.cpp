#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class MyObject2DComponent : public asd::Object2DComponent
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
		if (time % 6 == 0)
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
		: EngineTest(asd::ToAString("Component"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();
		auto component = make_shared<MyObject2DComponent>();

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		object->SetPosition(Vector2DF(320, 240));
		object->AddComponent(component, ToAString("rotation"));
		//ASSERT_NE(object->GetComponent(ToAString("rotation")), nullptr);

		auto g = asd::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(g);
	}
};

ENGINE_TEST(ObjectSystem, Component)
