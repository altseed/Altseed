#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_ObjectsUpdatePriority;

class MessageObject : public TextureObject2D
{
private:
	ObjectSystem_ObjectsUpdatePriority *m_parent;
	astring m_message;

protected:
	void OnUpdate() override;

public:
	MessageObject(ObjectSystem_ObjectsUpdatePriority *parent, astring message)
		: m_parent(parent), m_message(message)
	{
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Cloud1.png").c_str());
		SetTexture(texture);
	}
};

class ObjectSystem_ObjectsUpdatePriority : public EngineTest
{
private:
	std::vector<astring> m_messages;

public:
	ObjectSystem_ObjectsUpdatePriority(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ObjectsUpdatePriority"), isOpenGLMode, 60)
	{
	}

	void AddMessage(const astring &message)
	{
		m_messages.push_back(message);
	}

protected:
	void OnStart()
	{
		auto obj1 = make_shared<MessageObject>(this, ToAString("object1"));
		obj1->SetUpdatePriority(2);
		asd::Engine::AddObject2D(obj1);

		auto obj2 = make_shared<MessageObject>(this, ToAString("object2"));
		obj2->SetUpdatePriority(1);
		asd::Engine::AddObject2D(obj2);

		auto obj3 = make_shared<MessageObject>(this, ToAString("object3"));
		asd::Engine::AddObject2D(obj3);
		obj3->SetUpdatePriority(0);

		auto obj4 = make_shared<MessageObject>(this, ToAString("object4"));
		asd::Engine::AddObject2D(obj4);
		asd::Engine::RemoveObject2D(obj4);
		obj4->SetUpdatePriority(0);
	}

	void OnFinish()
	{
		ASSERT_EQ(3, m_messages.size());
		ASSERT_EQ(ToAString("object3"), m_messages[0]);
		ASSERT_EQ(ToAString("object2"), m_messages[1]);
		ASSERT_EQ(ToAString("object1"), m_messages[2]);
	}
};

void MessageObject::OnUpdate()
{
	m_parent->AddMessage(m_message);
	Dispose();
}

ENGINE_TEST(ObjectSystem, ObjectsUpdatePriority)