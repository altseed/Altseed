
#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_EffectObject2D : public EngineTest
{
public:
	Graphics_EffectObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("EffectObject2D"), isOpenGLMode, 20)
	{
	}

private:
	shared_ptr<EffectObject2D> effectObj;

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto effect = ace::Engine::GetGraphics()->CreateEffect(ace::ToAString("Data/Effect/Laser01.efk").c_str());
		effectObj = make_shared<EffectObject2D>();
	
		layer->AddObject(effectObj);
		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);

		effectObj->SetPosition(ace::Vector2DF(320, 240));
		effectObj->SetScale(ace::Vector2DF(20, 20));
		effectObj->SetEffectRotation(90);

		effectObj->SetEffect(effect);
		effectObj->Play();
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, EffectObject2D)
