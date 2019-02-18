#include "../../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace asd;

class Graphics_PlayEffectImmediately : public EngineTest
{
public:
	Graphics_PlayEffectImmediately(bool isOpenGLMode) : EngineTest(u"PlayEffectImmediately", isOpenGLMode, 60) {}

protected:
	void OnStart()
	{
		auto effect = Engine::GetGraphics()->CreateEffect(u"Data/Effect/Laser01.efk");
		auto effectObj = make_shared<EffectObject2D>();

		effectObj->SetPosition(asd::Vector2DF(320, 240));
		effectObj->SetScale(asd::Vector2DF(20, 20));
		effectObj->SetEffectRotation(90);
		effectObj->SetEffect(effect);

		Engine::AddObject2D(effectObj);

		effectObj->Play();
	}
};

ENGINE_TEST(Graphics, PlayEffectImmediately)