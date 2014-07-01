#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_Text2DTest : public EngineTest
{
public:
	ObjectSystem_Text2DTest(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Text2D"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		auto g = ace::Engine::GetGraphics();
		
		auto font = g->CreateFont(ace::ToAString("Data/Text/textTest.aff").c_str());

		object->SetFont(font);
		object->SetText(ToAString("Font"));

		//object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(320, 240));
	}
};


TEST(ObjectSystem, Text2D_GL)
{
	RunTest<ObjectSystem_Text2DTest>(true);
}

#if _WIN32
TEST(ObjectSystem, Text2D_DX)
{
	RunTest<ObjectSystem_Text2DTest>(false);
}
#endif

void ObjectSystem_Text2DTest_(bool isGL)
{
	ObjectSystem_Text2DTest(isGL).Run();
	AssertMemoryDoesntLeak();
}