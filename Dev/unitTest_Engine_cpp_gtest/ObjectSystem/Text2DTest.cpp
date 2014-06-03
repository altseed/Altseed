#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class Text2DTest : public EngineTest
{
public:
	Text2DTest(bool isOpenGLMode)
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
		
		auto font = g->CreateFont(ace::ToAString("hoge").c_str());

		object->SetCenterPosition(Vector2DF(128, 128));
		object->SetAngle(5);
		object->SetPosition(Vector2DF(320, 240));
		object->SetScale(object->GetScale() + Vector2DF(0.5f, 0.5f));
	}
};

/*
TEST(ObjectSystem, Simple_GL)
{
	RunTest<Text2DTest>(true);
}

#if _WIN32
TEST(ObjectSystem, Simple_DX)
{
	RunTest<Text2DTest>(false);
}
#endif
*/

