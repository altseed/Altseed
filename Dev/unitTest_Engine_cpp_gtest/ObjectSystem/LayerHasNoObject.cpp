#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace asd;
using namespace std;

class ObjectSystem_LayerHasNoObject : public EngineTest
{
public:
	ObjectSystem_LayerHasNoObject(bool isOpenGLMode)
		: EngineTest(asd::ToAString("LayerHasNoObject"), isOpenGLMode, 20)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
	}
};

ENGINE_TEST(ObjectSystem, LayerHasNoObject)
