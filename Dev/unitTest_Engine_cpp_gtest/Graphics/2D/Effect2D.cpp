
#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_Effect2D : public EngineTest
{
public:
	Graphics_Effect2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Effect2D"), isOpenGLMode, 60)
	{
	}

private:
	shared_ptr<EffectObject2D> m_effectObj;

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto effect = ace::Engine::GetGraphics()->CreateEffect(ace::ToAString("Data/Effect/Laser01.efk").c_str());
		m_effectObj = make_shared<EffectObject2D>();
	
		layer->AddObject(m_effectObj);
		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);

		m_effectObj->SetEffect(effect);
		m_effectObj->Play();
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, Effect2D)
