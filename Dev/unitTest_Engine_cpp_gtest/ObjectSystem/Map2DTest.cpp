#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class Map2DTest : public EngineTest
{
public:
	Map2DTest(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Map2DTest"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<MapObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		auto g = ace::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Map/MapTest.png").c_str());
		auto chip = g->CreateChip2D();
		chip->SetTexture(texture.get());
		chip->SetSrc(ace::RectF(0, 0, 256, 256));
		object->AddChip(chip);

		object->SetPosition(Vector2DF(0, 0));
	}
};


TEST(ObjectSystem, Map2D_GL)
{
	RunTest<Map2DTest>(true);
}

#if _WIN32
TEST(ObjectSystem, Map2D_DX)
{
	RunTest<Map2DTest>(false);
}
#endif

void ObjectSystem_Map2DTest_(bool isGL)
{
	Map2DTest(isGL).Run();
	AssertMemoryDoesntLeak();
}