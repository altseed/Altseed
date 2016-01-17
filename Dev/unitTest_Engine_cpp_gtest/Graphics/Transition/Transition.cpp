
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

class Graphics_Transition : public EngineTest
{
public:
	Graphics_Transition(bool isOpenGLMode)
		: EngineTest(asd::ToAString("Transition"), isOpenGLMode, 150)
	{
	}

private:

protected:
	void OnStart()
	{
	}

	void OnUpdating()
	{
		if (GetTime() == 9)
		{
			auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

			auto obj = std::make_shared<asd::TextureObject2D>();
			obj->SetTexture(texture);
			auto scene = std::make_shared<asd::Scene>();
			auto layer = std::make_shared<asd::Layer2D>();
			layer->AddObject(obj);
			scene->AddLayer(layer);

			auto transition = std::make_shared<asd::TransitionFade>(60.0f/60.0f, 60.0f/60.0f);
			asd::Engine::ChangeSceneWithTransition(scene, transition);
		}
	}
};


ENGINE_TEST(Graphics, Transition)
