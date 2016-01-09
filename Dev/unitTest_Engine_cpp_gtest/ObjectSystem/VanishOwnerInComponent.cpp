#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_VanishOwnerInComponent : public EngineTest
{
	class VanishingComponent : public Object2DComponent
	{
		int eTime, count = 0;

	public:
		VanishingComponent(int eTime)
		{
			this->eTime = eTime;
		}

	protected:
		void OnUpdate()
		{
			GetOwner()->SetPosition(GetOwner()->GetPosition() + asd::Vector2DF(1, 0));

			count++;
			if (eTime == count)
			{
				printf("vanish\n");
				GetOwner()->Dispose();
			}
		}
	};

	//shared_ptr<Layer2D> layer = nullptr;
	//shared_ptr<TextureObject2D> obj = nullptr;

public:
	ObjectSystem_VanishOwnerInComponent(bool isOpenGLMode)
		: EngineTest(asd::ToAString("VanishOwnerInComponent"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto obj = std::make_shared<TextureObject2D>();

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);

		obj->AddComponent(make_shared<VanishingComponent>(10), ToAString("Vanish"));
	}

	void OnUpdating()
	{
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(ObjectSystem, VanishOwnerInComponent)