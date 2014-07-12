#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_TextObject2D : public EngineTest
{
public:
	Graphics_TextObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("TextObject2D"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextObject2D>();
		auto back = make_shared<TextureObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);
		ASSERT_NE(back, nullptr);

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto g = ace::Engine::GetGraphics();

		layer->AddObject(back);
		back->SetTexture(g->CreateTexture2D(ace::ToAString("Data/Text/back.png").c_str()));

		layer->AddObject(object);
		auto font = g->CreateFont(ace::ToAString("Data/Text/Nac0711.aff").c_str());

		object->SetFont(font);
		object->SetText(ToAString("DPは依存関係\nプロパティ"));

		//object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(0, 0));
	}
};


ENGINE_TEST(Graphics, TextObject2D)
