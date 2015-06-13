#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_VanishInComponent : public EngineTest
{
	class VanishingComponent 
		: public asd::Object2DComponent
	{
		int time = 0;

		asd::Object2D::Ptr vanishedObject = nullptr;
		int etime = 0;

	public :
		VanishingComponent(asd::Object2D::Ptr vanishedObject, int etime)
		{
			this->vanishedObject = vanishedObject;
			this->etime = etime;
		}

	protected:
		void OnUpdate()
		{
			if (time == etime)
			{
				vanishedObject->Vanish();
			}

			time++;
		}
	};

	std::shared_ptr<asd::Layer2D> layer = nullptr;
	std::shared_ptr<asd::Object2D> obj = nullptr;

public:
	ObjectSystem_VanishInComponent(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ObjectSystem_VanishInComponent"), isOpenGLMode, 40)
	{
	}

protected :
	void OnStart()
	{
		auto scene = std::make_shared<asd::Scene>();
		layer = std::make_shared<asd::Layer2D>();
		obj = std::make_shared<asd::TextureObject2D>();

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);
	}

	void OnUpdating()
	{
		if (GetTime() % 2 == 0)
		{
			auto temp = obj;
			obj = std::make_shared<asd::TextureObject2D>();
			layer->AddObject(obj);
			obj->AddComponent(std::make_shared<VanishingComponent>(temp, 3), asd::ToAString("v").c_str());
		}
	}
};


ENGINE_TEST(ObjectSystem, VanishInComponent)