
#include "../../EngineGraphics3DTest.h"

class Graphics_Model : public EngineGraphics3DTest
{

public:

	Graphics_Model(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("Model"), isOpenGLMode, 6000,true)
	{}

protected:
	std::shared_ptr<ace::ModelObject3D> meshObj;


	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		meshObj = std::make_shared<ace::ModelObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();

		auto graphics = ace::Engine::GetGraphics();

		auto model = graphics->CreateModel(ace::ToAString("Data/Model/binormals.mdl").c_str());

		meshObj->SetModel(model);
		meshObj->SetPosition(ace::Vector3DF(0, 0, 0));
		lightObj->SetRotation(ace::Vector3DF(120, 50, 0));

		GetLayer3D()->AddObject(meshObj);
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(20, 0, 0, 1, 40, 20);

		meshObj->PlayAnimation(ace::ToAString("Constant").c_str());
		
	}

	void OnUpdating() override
	{
		EngineGraphics3DTest::OnUpdating();
		//auto rot = meshObj->GetRotation();
		//meshObj->SetRotation(rot+ace::Vector3DF(5,0,0));
	}


};


TEST(Graphics, Model_GL)
{
	Graphics_Model(true).Run();
	AssertMemoryDoesntLeak();
}

#if _WIN32
TEST(Graphics, Model_DX)
{
	Graphics_Model(false).Run();
	AssertMemoryDoesntLeak();
}
#endif

void Graphics_Model_(bool isGL)
{
	Graphics_Model(isGL).Run();
	AssertMemoryDoesntLeak();
}

