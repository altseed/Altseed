
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_EffectObject2D : public EngineTest
{
public:
	Graphics_EffectObject2D(bool isOpenGLMode)
		: EngineTest(asd::ToAString("EffectObject2D"), isOpenGLMode, 20)
	{
	}

private:
	shared_ptr<EffectObject2D> effectObj;

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto effect = asd::Engine::GetGraphics()->CreateEffect(asd::ToAString("Data/Effect/Laser01.efk").c_str());
		effectObj = make_shared<EffectObject2D>();
	
		layer->AddObject(effectObj);
		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);

		effectObj->SetPosition(asd::Vector2DF(320, 240));
		effectObj->SetScale(asd::Vector2DF(20, 20));
		effectObj->SetEffectRotation(90);

		effectObj->SetEffect(effect);
	}

	void OnUpdating()
	{
		if (m_currentTime == 1)
		{
			effectObj->Play();
		}
	}
};


ENGINE_TEST(Graphics, EffectObject2D)
