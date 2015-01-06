#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_VanishInComponent : public EngineTest
{
	class MyComponent : public Object2DComponent
	{
		Object2D::Ptr forVanish;

	public:
		MyComponent(Object2D::Ptr object)
		{
			forVanish = object;
		}

		void OnUpdate()
		{
			forVanish->Vanish();
		}
	};

public:
	ObjectSystem_VanishInComponent(bool isOpenGLMode)
		: EngineTest(ace::ToAString("ObjectSystem_VanishInComponent"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();
		auto object2 = make_shared<TextureObject2D>();

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto g = ace::Engine::GetGraphics();

		ace::Engine::GetFile()->AddRootDirectories(ace::ToAString("Data").c_str());

		auto texture = g->CreateTexture2D(L"Texture/Cloud1.png");
		object->SetTexture(texture);
		object2->SetTexture(texture);

		auto component = make_shared<MyComponent>(object2);
		object->AddComponent(component, ace::ToAString("Component"));
	}
};


ENGINE_TEST(ObjectSystem, VanishInComponent)