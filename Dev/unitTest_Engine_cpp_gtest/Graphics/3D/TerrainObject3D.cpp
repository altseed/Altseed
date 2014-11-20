
#include "../../EngineGraphics3DTest.h"

class Graphics_TerrainObject3D : public EngineGraphics3DTest
{
public:

	Graphics_TerrainObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("TerrainObject3D"), isOpenGLMode, true, 20)
	{}

protected:
	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = ace::Engine::GetGraphics();

		



		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		lightObj->SetRotation(ace::Vector3DF(120, 50, 0));
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(30, 0, 0, 1, 60, 20);
		GetLayer3D()->SetSkyAmbientColor(ace::Color(50, 50, 70, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(70, 70, 50, 255));
	}
};

//ENGINE_TEST(Graphics, TerrainObject3D)