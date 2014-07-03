#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_MapObject2D : public EngineTest
{
public:
	Graphics_MapObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("MapObject2D"), isOpenGLMode, 30)
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
		auto chip1 = g->CreateChip2D();
		chip1->SetTexture(texture.get());
		chip1->SetSrc(ace::RectF(0, 0, 256, 256));
		object->AddChip(chip1);

		auto chip2 = g->CreateChip2D();
		chip2->SetTexture(texture.get());
		chip2->SetSrc(ace::RectF(300, 0, 256, 256));
		object->AddChip(chip2);

		object->SetPosition(Vector2DF(0, 0));
	}
};

ENGINE_TEST(Graphics, MapObject2D)