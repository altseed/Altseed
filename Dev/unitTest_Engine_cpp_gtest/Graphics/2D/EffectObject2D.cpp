
#include "../../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>

using namespace std;
using namespace asd;

class Graphics_EffectObject2D : public EngineTest
{
public:
	Graphics_EffectObject2D(bool isOpenGLMode) : EngineTest(u"EffectObject2D", isOpenGLMode, 20) {}

private:
	shared_ptr<EffectObject2D> effectObj;

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto effect = asd::Engine::GetGraphics()->CreateEffect(u"Data/Effect/Laser01.efk");
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
