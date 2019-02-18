#include "../../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace asd;

class Graphics_TextObject2D : public EngineTest
{
public:
	Graphics_TextObject2D(bool isOpenGLMode) : EngineTest(u"TextObject2D", isOpenGLMode, 10) {}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto g = asd::Engine::GetGraphics();

		asd::Engine::GetFile()->AddRootDirectory(u"Data");

		layer->AddObject(object);
		auto font = g->CreateFont(u"Font/Nac0812.aff");

		object->SetText(u"DependencyPropertyとは");

		// object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(0, 0));
		object->SetFont(font);

		auto size = font->CalcTextureSize(u"DependencyPropertyとは", WritingDirection::Horizontal);

		printf("%d %d\n", size.X, size.Y);
	}
};

ENGINE_TEST(Graphics, TextObject2D)
