#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"
#include <iostream>

using namespace std;
using namespace asd;

class ObjectSystem_DisposeOnVanish : public EngineTest
{
private:
	class MessageLayer2D : public Layer2D
	{
	protected:
		void OnDispose()
		{
			wprintf(L"Layer2D(%s):Dispose\n", GetName().c_str());
		}
	};

	class MessageObject2D : public TextureObject2D
	{
	public:
		astring m_name;

	protected:
		void OnDispose()
		{
			wprintf(L"Object2D(%s):Dispose\n", m_name.c_str());
		}
	};

	int time;
	shared_ptr<MessageLayer2D> layer1;
	shared_ptr<MessageObject2D> object1;

public:
	ObjectSystem_DisposeOnVanish(bool isOpenGLMode)
		: EngineTest(asd::ToAString("DisposeOnVanish"), isOpenGLMode, 120)
		, time(0)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		layer1 = make_shared<MessageLayer2D>();
		auto layer2 = make_shared<MessageLayer2D>();
		object1 = make_shared<MessageObject2D>();
		auto object2 = make_shared<MessageObject2D>();

		Engine::ChangeScene(scene);
		scene->AddLayer(layer1);
		scene->AddLayer(layer2);
		layer1->AddObject(object1);
		layer1->AddObject(object2);

		layer1->SetName(ToAString("Layer1"));
		layer2->SetName(ToAString("Layer2"));
		object1->m_name = ToAString("Object1");
		object2->m_name = ToAString("Object2");
	}

	void OnUpdated()
	{
		time++;
		if (time == 10)
		{
			printf("Vanish Object1\n");
			object1->Vanish();
		}
		if (time == 60)
		{
			printf("Vanish Layer1\n");
			layer1->Vanish();
		}
	}

	void OnFinish()
	{
		printf("Terminate\n");
	}
};

ENGINE_TEST(ObjectSystem, DisposeOnVanish)