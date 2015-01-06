#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_VanishInComponent : public EngineTest
{
	class VanishingComponent 
		: public ace::Object2DComponent
	{
		int time = 0;

		ace::Object2D::Ptr vanishedObject = nullptr;
		int etime = 0;

	public :
		VanishingComponent(ace::Object2D::Ptr vanishedObject, int etime)
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

	std::shared_ptr<ace::Layer2D> layer = nullptr;
	std::shared_ptr<ace::Object2D> obj = nullptr;

public:
	ObjectSystem_VanishInComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("ObjectSystem_VanishInComponent"), isOpenGLMode, 40)
	{
	}

protected :
	void OnStart()
	{
		auto scene = std::make_shared<ace::Scene>();
		layer = std::make_shared<ace::Layer2D>();
		obj = std::make_shared<ace::TextureObject2D>();

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);
	}

	void OnUpdating()
	{
		if (GetTime() % 2 == 0)
		{
			auto temp = obj;
			obj = std::make_shared<ace::TextureObject2D>();
			layer->AddObject(obj);
			obj->AddComponent(std::make_shared<VanishingComponent>(temp, 3), ace::ToAString("v").c_str());
		}
	}
};


ENGINE_TEST(ObjectSystem, VanishInComponent)