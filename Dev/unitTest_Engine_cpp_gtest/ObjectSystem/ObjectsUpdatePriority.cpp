#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class MessageObject : public TextureObject2D
{
private:
	astring m_message;

protected:
	void OnUpdate() override
	{
		wprintf((m_message + L"\n").c_str());
		Vanish();
	}

public:
	MessageObject(astring message)
		: m_message(message)
	{
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Cloud1.png").c_str());
		SetTexture(texture);
	}
};

class ObjectSystem_ObjectsUpdatePriority : public EngineTest
{
public:
	ObjectSystem_ObjectsUpdatePriority(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ObjectsUpdatePriority"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto obj1 = make_shared<MessageObject>(ToAString("object1"));
		obj1->SetUpdatePriority(2);
		asd::Engine::AddObject2D(obj1);

		auto obj2 = make_shared<MessageObject>(ToAString("object2"));
		obj2->SetUpdatePriority(1);
		asd::Engine::AddObject2D(obj2);

		auto obj3 = make_shared<MessageObject>(ToAString("object3"));
		asd::Engine::AddObject2D(obj3);
		obj3->SetUpdatePriority(0);

		auto obj4 = make_shared<MessageObject>(ToAString("object4"));
		asd::Engine::AddObject2D(obj4);
		asd::Engine::RemoveObject2D(obj4);
		obj4->SetUpdatePriority(0);
	}
};

ENGINE_TEST(ObjectSystem, ObjectsUpdatePriority)