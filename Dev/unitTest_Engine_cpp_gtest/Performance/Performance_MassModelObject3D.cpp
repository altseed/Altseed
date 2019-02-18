
#include "../EngineGraphics3DTest.h"

class Performance_MassModelObject3D : public EngineGraphics3DTest
{

public:
	Performance_MassModelObject3D(bool isOpenGLMode)
		: EngineGraphics3DTest(asd::ToAString("Performance_MassModelObject3D"),
							   isOpenGLMode, 150, true)
	{
	}

protected:
	std::vector<std::shared_ptr<asd::MassModelObject3D>> meshObjs;

	void OnStart() override
	{
		asd::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = asd::Engine::GetGraphics();

		auto massModel = graphics->CreateMassModelFromModelFile(
			asd::ToAString("Data/Model/Sphere1.mdl").c_str());

		for (int y = -5; y <= 5; y++)
		{
			for (int x = -5; x <= 5; x++)
			{
				auto m = std::make_shared<asd::MassModelObject3D>();
				m->SetModel(massModel);

				m->SetPosition(asd::Vector3DF(2 * x, 0, 2 * y));
				GetLayer3D()->AddObject(m);

				meshObjs.push_back(m);
			}
		}

		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		lightObj->SetRotation(asd::Vector3DF(120, 50, 0));
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(30, 0, 0, 1, 60, 20);
		GetLayer3D()->SetSkyAmbientColor(asd::Color(50, 50, 70, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(70, 70, 50, 255));
	}

	void OnUpdating() override { EngineGraphics3DTest::OnUpdating(); }
};

#if defined(PERFORMANCE_MODE)
ENGINE_TEST(Performance, MassModelObject3D)
#endif
