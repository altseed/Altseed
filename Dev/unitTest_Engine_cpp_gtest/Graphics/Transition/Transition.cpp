
#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

class Graphics_Transition : public EngineTest
{
public:
	Graphics_Transition(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Transition"), isOpenGLMode, 20)
	{
	}

private:

protected:
	void OnStart()
	{
	}

	void OnUpdating()
	{
		if (GetTime() % 9 == 0)
		{
			auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());

			auto obj = std::make_shared<ace::TextureObject2D>();
			obj->SetTexture(texture);
			auto scene = std::make_shared<ace::Scene>();
			auto layer = std::make_shared<ace::Layer2D>();
			layer->AddObject(obj);
			scene->AddLayer(layer);

			auto transition = std::make_shared<ace::TransitionFade>(3.0f/60.0f, 3.0f/60.0f);
			ace::Engine::ChangeSceneWithTransition(scene, transition);
		}
	}
};


ENGINE_TEST(Graphics, Transition)
