
#include "../../EngineGraphics3DTest.h"

class Graphics_Model : public EngineGraphics3DTest
{
	std::shared_ptr<ace::ModelObject3D> meshObj;
public:

	Graphics_Model(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("Model"), isOpenGLMode, 60,true)
	{}

protected:
	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = true;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		meshObj = std::make_shared<ace::ModelObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();

		auto graphics = ace::Engine::GetGraphics();

		auto model = graphics->CreateModel(ace::ToAString("Data/Model/out.mdl").c_str());


		meshObj->SetModel(model);
		meshObj->SetPosition(ace::Vector3DF(5, 5, 0));
		lightObj->SetRotation(ace::Vector3DF(180, 0, 0));

		GetLayer3D()->AddObject(meshObj);
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(10, 0, 0, 1, 20, 20);
	}

	void OnUpdating() override
	{
		EngineGraphics3DTest::OnUpdating();
		auto rot = meshObj->GetRotation();
		meshObj->SetRotation(rot + ace::Vector3DF(3, 0, 0));
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

