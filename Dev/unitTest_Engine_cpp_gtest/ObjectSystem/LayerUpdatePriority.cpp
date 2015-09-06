#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_LayerUpdatePriority;

class MessageLayer : public Layer2D
{
private:
	ObjectSystem_LayerUpdatePriority *m_parent;
	astring m_message;

protected:
	void OnUpdating() override;

public:
	MessageLayer(ObjectSystem_LayerUpdatePriority *parent, astring message)
		: m_parent(parent), m_message(message)
	{
	}
};

class ObjectSystem_LayerUpdatePriority : public EngineTest
{
private:
	std::vector<astring> m_messages;

public:
	ObjectSystem_LayerUpdatePriority(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LayerUpdatePriority"), isOpenGLMode, 60)
	{
	}

	void AddMessage(const astring &message) {
		m_messages.push_back(message);
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer1 = make_shared<MessageLayer>(this, ToAString("Layer1"));
		layer1->SetUpdatePriority(1);

		auto layer2 = make_shared<MessageLayer>(this, ToAString("Layer2"));
		layer2->SetUpdatePriority(0);

		scene->AddLayer(layer1);
		scene->AddLayer(layer2);
		asd::Engine::ChangeScene(scene);
	}

	void OnFinish()
	{
		ASSERT_EQ(2, m_messages.size());
		ASSERT_EQ(ToAString("Layer2"), m_messages[0]);
		ASSERT_EQ(ToAString("Layer1"), m_messages[1]);
	}
};

void MessageLayer::OnUpdating()
{
	m_parent->AddMessage(m_message);
	Vanish();
}

ENGINE_TEST(ObjectSystem, LayerUpdatePriority)