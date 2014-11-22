
#include "../../EngineGraphics3DTest.h"

class Graphics_TerrainObject3D : public EngineGraphics3DTest
{
public:

	Graphics_TerrainObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("TerrainObject3D"), isOpenGLMode, 20000, true)
	{}

protected:
	void OnStart() override
	{
		ace::RenderSettings settings;
		settings.IsLightweightMode = false;
		settings.VisualizedBuffer = ace::VisualizedBufferType::FinalImage;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = ace::Engine::GetGraphics();

		auto t = graphics->CreateTerrain3D();
		t->New(1, 16, 16);
		t->AddSurface(ace::ToAString("sf1").c_str(), 1, ace::ToAString("sf1").c_str(), ace::ToAString("sf1").c_str(), ace::ToAString("sf1").c_str());

		auto tObj = std::make_shared<ace::TerrainObject3D>();
		tObj->SetTerrain(t);
		GetLayer3D()->AddObject(tObj);

		//auto plainObj = std::make_shared<ace::ModelObject3D>();
		//GetLayer3D()->AddObject(plainObj);
		//auto plainModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Plain1.mdl").c_str());
		//plainObj->SetModel(plainModel);

		auto sphereObj = std::make_shared<ace::ModelObject3D>();
		auto sphereModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Sphere1.mdl").c_str());
		sphereObj->SetModel(sphereModel);
		sphereObj->SetPosition(ace::Vector3DF(0.0f, 1.0f, 0.0f));
		GetLayer3D()->AddObject(sphereObj);

		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		lightObj->SetRotation(ace::Vector3DF(120, 50, 0));
		GetLayer3D()->AddObject(lightObj);

		GetLayer3D()->SetSkyAmbientColor(ace::Color(50, 50, 70, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(70, 70, 50, 255));

		SetCameraParameter(10, 15, -15, 1, 200, 20);
	}
};

//ENGINE_TEST(Graphics, TerrainObject3D)