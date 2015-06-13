
#include "../../EngineTest.h"

class Graphics_EffectObject3D : public EngineTest
{
public:

	Graphics_EffectObject3D(bool isOpenGLMode) :
		EngineTest(asd::ToAString("EffectObject3D"), isOpenGLMode, 20)
	{}

protected:
	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer3D>();
		auto effectObj = std::make_shared<asd::EffectObject3D>();
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		auto cameraObj = std::make_shared<asd::CameraObject3D>();
		auto effect = asd::Engine::GetGraphics()->CreateEffect(asd::ToAString("Data/Effect/Laser01.efk").c_str());

		scene->AddLayer(layer);
		layer->AddObject(effectObj);
		layer->AddObject(lightObj);
		layer->AddObject(cameraObj);
		asd::Engine::ChangeScene(scene);

		cameraObj->SetPosition(asd::Vector3DF(40, 20, 40));
		cameraObj->SetFocus(asd::Vector3DF(0, 0, 0));
		cameraObj->SetFieldOfView(50.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(200.0f);
		cameraObj->SetWindowSize(asd::Vector2DI(800, 600));

		effectObj->SetRotation(asd::Vector3DF(00.0f, 20.0f, 0.0f));

		lightObj->SetRotation(asd::Vector3DF(30, 160, 0));

		effectObj->SetEffect(effect);
		effectObj->Play();
	}
};

ENGINE_TEST(Graphics, EffectObject3D)