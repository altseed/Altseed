#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class MessageLayer : public Layer2D
{
private:
	astring m_message;

protected:
	void OnUpdating() override
	{
		wprintf((m_message + L"\n").c_str());
		Vanish();
	}

public:
	MessageLayer(astring message)
		: m_message(message)
	{
	}
};

class ObjectSystem_LayerUpdatePriority : public EngineTest
{
public:
	ObjectSystem_LayerUpdatePriority(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LayerUpdatePriority"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer1 = make_shared<MessageLayer>(ToAString("Layer1"));
		layer1->SetUpdatePriority(1);

		auto layer2 = make_shared<MessageLayer>(ToAString("Layer2"));
		layer2->SetUpdatePriority(0);

		scene->AddLayer(layer1);
		scene->AddLayer(layer2);
		asd::Engine::ChangeScene(scene);
	}
};

ENGINE_TEST(ObjectSystem, LayerUpdatePriority)