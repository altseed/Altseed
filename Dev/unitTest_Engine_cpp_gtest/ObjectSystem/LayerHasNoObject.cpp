#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace ace;
using namespace std;

class ObjectSystem_LayerHasNoObject : public EngineTest
{
public:
	ObjectSystem_LayerHasNoObject(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayerHasNoObject"), isOpenGLMode, 20)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
	}
};

TEST(ObjectSystem, LayerHasNoObject_GL)
{
	RunTest<ObjectSystem_LayerHasNoObject>(true);
}

#if _WIN32
TEST(ObjectSystem, LayerHasNoObject_DX)
{
	RunTest<ObjectSystem_LayerHasNoObject>(false);
}
#endif