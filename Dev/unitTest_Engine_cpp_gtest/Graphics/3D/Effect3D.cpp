
#include "../../EngineTest.h"

class Graphics_Effect3D : public EngineTest
{
public:

	Graphics_Effect3D(bool isOpenGLMode) :
		EngineTest(ace::ToAString("Effect3D"), isOpenGLMode, 20)
	{}

protected:
	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer3D>();
		auto effectObj = std::make_shared<ace::EffectObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		auto cameraObj = std::make_shared<ace::CameraObject3D>();
		auto effect = ace::Engine::GetGraphics()->CreateEffect(ace::ToAString("Data/Effect/Laser01.efk").c_str());

		scene->AddLayer(layer);
		layer->AddObject(effectObj);
		layer->AddObject(lightObj);
		layer->AddObject(cameraObj);
		ace::Engine::ChangeScene(scene);

		cameraObj->SetPosition(ace::Vector3DF(40, 20, 40));
		cameraObj->SetFocus(ace::Vector3DF(0, 0, 0));
		cameraObj->SetFieldOfView(50.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(200.0f);
		cameraObj->SetWindowSize(ace::Vector2DI(800, 600));

		effectObj->SetRotation(ace::Vector3DF(00.0f, 20.0f, 0.0f));

		lightObj->SetRotation(ace::Vector3DF(30, 160, 0));

		effectObj->SetEffect(effect);
		effectObj->Play();
	}
};


TEST(Graphics, Effect3D_GL)
{
	Graphics_Effect3D(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Graphics, Effect3D_DX)
{
	Graphics_Effect3D(false).Run();
	AssertMemoryDoesntLeak();
}
#endif

void Graphics_Effect3D_(bool isGL)
{
	Graphics_Effect3D(isGL).Run();
	AssertMemoryDoesntLeak();
}
