#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_TextObject2D_Dynamic : public EngineTest
{
public:
	Graphics_TextObject2D_Dynamic(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TextObject2D_Dynamic"), isOpenGLMode, 10)
	{
	}

protected:
	void OnStart()
	{
		auto object = make_shared<TextObject2D>();
		asd::Engine::AddObject2D(object);

		ASSERT_NE(object, nullptr);

		auto g = asd::Engine::GetGraphics();
#ifdef __APPLE__
		auto font = g->CreateDynamicFont(asd::ToAString("Helvetica").c_str(), 32, Color(255, 0, 0, 255), 0, Color(0, 0, 0, 0));
#else
		auto font = g->CreateDynamicFont(asd::ToAString("").c_str(), 32, Color(255, 0, 0, 255), 0, Color(0, 0, 0, 0));
#endif

		object->SetText(ToAString("DependencyPropertyとは").c_str());

		//object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(0, 0));
		object->SetFont(font);

		auto size = font->CalcTextureSize(ToAString("DependencyPropertyとは").c_str(), WritingDirection::Horizontal);

		printf("%d %d\n", size.X,size.Y);
	}
};


ENGINE_TEST(Graphics, TextObject2D_Dynamic)
