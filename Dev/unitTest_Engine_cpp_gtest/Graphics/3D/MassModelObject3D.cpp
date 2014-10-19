
#include "../../EngineGraphics3DTest.h"

class Graphics_MassModelObject3D : public EngineGraphics3DTest
{

public:

	Graphics_MassModelObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("MassModelObject3D"), isOpenGLMode, 1500, true)
	{}

protected:
	std::shared_ptr<ace::MassModelObject3D> meshObjs;


	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();


		auto graphics = ace::Engine::GetGraphics();

		auto massModel = graphics->CreateMassModelFromModelFile(ace::ToAString("Data/Model/MassTest1.mdl").c_str());
		
		
		auto m = std::make_shared<ace::MassModelObject3D>();
		m->SetModel(massModel);
		
		m->SetPosition(ace::Vector3DF(0, 0, 0));
		GetLayer3D()->AddObject(m);
		
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		lightObj->SetRotation(ace::Vector3DF(120, 50, 0));
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(30, 0, 0, 1, 60, 20);
		GetLayer3D()->SetSkyAmbientColor(ace::Color(50, 50, 70, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(70, 70, 50, 255));
		
	}

	void OnUpdating() override
	{
		EngineGraphics3DTest::OnUpdating();
	}
};

//ENGINE_TEST(Graphics, MassModelObject3D)