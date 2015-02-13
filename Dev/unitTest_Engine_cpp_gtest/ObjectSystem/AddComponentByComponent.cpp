#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_AddComponentByComponent : public EngineTest
{
	class AddingObjectComponent : public Object2DComponent
	{
		int count = 0;
	protected:
		void OnUpdate() override
		{
			count++;
			if (count == 10)
			{
				GetOwner()->AddComponent(make_shared<AddingObjectComponent>(), ToAString("Add"));
				printf("vanish object component.\n");
				Vanish();
			}
		}
	};

	class AddingLayerComponent : public Layer2DComponent
	{
		int count = 0;
	protected:
		void OnUpdate() override
		{
			count++;
			if (count == 10)
			{
				GetOwner()->AddComponent(make_shared<AddingLayerComponent>(), ToAString("Add"));
				Vanish();
			}
		}
	};

	class AddingSceneComponent : public SceneComponent
	{
		int count = 0;
	protected:
		void OnUpdate() override
		{
			count++;
			if (count == 10)
			{
				GetOwner()->AddComponent(make_shared<AddingSceneComponent>(), ToAString("Add"));
				Vanish();
			}
		}
	};

public:
	ObjectSystem_AddComponentByComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("AddComponentByComponent"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto obj = std::make_shared<ace::TextureObject2D>();

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);

		//scene->AddComponent(make_shared<AddingSceneComponent>(), ToAString("Add"));
		//layer->AddComponent(make_shared<AddingLayerComponent>(), ToAString("Add"));
		obj->AddComponent(make_shared<AddingObjectComponent>(), ToAString("Add"));
	}
};

ENGINE_TEST(ObjectSystem, AddComponentByComponent)