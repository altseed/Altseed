#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

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
				printf("vanish object component.\n");
				Dispose();
				GetOwner()->AddComponent(make_shared<AddingObjectComponent>(), u"Add");
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
				printf("vanish layer component.\n");
				Dispose();
				GetOwner()->AddComponent(make_shared<AddingLayerComponent>(), u"Add");
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
				printf("vanish scene component.\n");
				Dispose();
				GetOwner()->AddComponent(make_shared<AddingSceneComponent>(), u"Add");
			}
		}
	};

public:
	ObjectSystem_AddComponentByComponent(bool isOpenGLMode)
		: EngineTest(u"AddComponentByComponent", isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto obj = std::make_shared<asd::TextureObject2D>();

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);

		scene->AddComponent(make_shared<AddingSceneComponent>(), u"Add");
		layer->AddComponent(make_shared<AddingLayerComponent>(), u"Add");
		obj->AddComponent(make_shared<AddingObjectComponent>(), u"Add");
	}
};

ENGINE_TEST(ObjectSystem, AddComponentByComponent)