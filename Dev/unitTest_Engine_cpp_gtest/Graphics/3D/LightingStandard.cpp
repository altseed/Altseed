
#include "../../EngineGraphics3DTest.h"

class Graphics_LightingStandard : public EngineGraphics3DTest
{
public:

	Graphics_LightingStandard(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("LightingStandard"), isOpenGLMode, 15, true)
	{}

protected:

	void OnStart() override
	{
		ace::RenderSettings settings;
		//settings.IsLightweightMode = true;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto plainObj = std::make_shared<ace::ModelObject3D>();
		auto sphereObj = std::make_shared<ace::ModelObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();

		GetLayer3D()->AddObject(plainObj);
		GetLayer3D()->AddObject(sphereObj);
		GetLayer3D()->AddObject(lightObj);

		auto plainModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Plain1.mdl").c_str());
		auto sphereModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Sphere1.mdl").c_str());

		plainObj->SetModel(plainModel);
		sphereObj->SetModel(sphereModel);
		sphereObj->SetPosition(ace::Vector3DF(0.0f, 1.0f, 0.0f));
		
		lightObj->SetRotation(ace::Vector3DF(30, 160, 0));

		SetCameraParameter(10, 15, -15, 1, 200, 20);

		// テクスチャ
		auto texWhite = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/White.png").c_str());
		auto texBlack = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/Black.png").c_str());

		// 球素材
		auto mesh = sphereModel->GetMesh(0);
		mesh->SetColorTexture(0,texWhite);
		mesh->SetSpecularTexture(0, texWhite);
		// 環境光
#if 0
		GetLayer3D()->SetSkyAmbientColor(ace::Color(80, 80, 120, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(120, 80, 80, 255));
#else
		GetLayer3D()->SetSkyAmbientColor(ace::Color(0, 0, 0, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(0, 0, 0, 255));
#endif

	}

	void OnUpdating()
	{
		EngineGraphics3DTest::OnUpdating();
	}
};

ENGINE_TEST(Graphics, LightingStandard)