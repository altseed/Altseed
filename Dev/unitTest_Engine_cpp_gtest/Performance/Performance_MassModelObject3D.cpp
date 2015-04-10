
#include "../EngineGraphics3DTest.h"

class Performance_MassModelObject3D : public EngineGraphics3DTest
{

public:

	Performance_MassModelObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("Performance_MassModelObject3D"), isOpenGLMode, 1500000, true)
	{}

protected:
	std::vector<std::shared_ptr<ace::MassModelObject3D>> meshObjs;


	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = ace::Engine::GetGraphics();

		auto massModel = graphics->CreateMassModelFromModelFile(ace::ToAString("Data/Model/Sphere1.mdl").c_str());

		for (int y = -5; y <= 5; y++)
		{
			for (int x = -5; x <= 5; x++)
			{
				auto m = std::make_shared<ace::MassModelObject3D>();
				m->SetModel(massModel);

				m->SetPosition(ace::Vector3DF(2 * x, 0, 2 * y));
				GetLayer3D()->AddObject(m);

				meshObjs.push_back(m);
			}
		}

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

//ENGINE_TEST(Performance, MassModelObject3D)
