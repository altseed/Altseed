
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
		settings.VisualizedBuffer = ace::VisualizedBufferType::DiffuseColor;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = ace::Engine::GetGraphics();

		// 地形
		auto t = graphics->CreateTerrain3D();
		t->New(1, 16, 16);
		t->AddSurface(
			ace::ToAString("sf1").c_str(), 
			2,
			ace::ToAString("Data/Texture/Surface/Tile_Color.png").c_str(), 
			ace::ToAString("Data/Texture/Surface/Tile_Normal.png").c_str(),
			ace::ToAString("Data/Texture/Surface/Tile_Spec.png").c_str());

		t->AddSurface(
			ace::ToAString("sf2").c_str(),
			2,
			ace::ToAString("Data/Texture/Surface/Cloud_Color.png").c_str(),
			ace::ToAString("Data/Texture/Surface/Cloud_Normal.png").c_str(),
			ace::ToAString("Data/Texture/Surface/Cloud_Spec.png").c_str());

		auto sf2ind = t->GetSurfaceIndex(ace::ToAString("sf2").c_str());
		t->AssignSurfaceWithCircle(sf2ind, 0, 0, 2, 255, 0.5);

		auto tObj = std::make_shared<ace::TerrainObject3D>();
		tObj->SetTerrain(t);
		GetLayer3D()->AddObject(tObj);

		// 球
		auto sphereObj = std::make_shared<ace::ModelObject3D>();
		auto sphereModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Sphere1.mdl").c_str());
		sphereObj->SetModel(sphereModel);
		sphereObj->SetPosition(ace::Vector3DF(0.0f, 1.0f, 0.0f));
		//GetLayer3D()->AddObject(sphereObj);

		// 環境
		auto cubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFrom6ImageFiles(
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Front.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Left.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Back.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Right.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Top.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Bottom.png").c_str()
			);

		auto specCubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFromMipmapImageFiles(ace::ToAString("Data/Cubemap/Sky1/Spec/sky").c_str(), 8);

		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();
		lightObj->SetRotation(ace::Vector3DF(30, 160, 0));
		lightObj->SetColor(ace::Color(255, 255, 255, 200));
		GetLayer3D()->AddObject(lightObj);

		// 環境
		GetLayer3D()->SetEnvironmentColor(cubemap, specCubemap);
		// 環境光
#if 0
		GetLayer3D()->SetSkyAmbientColor(ace::Color(10, 10, 20, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(20, 10, 10, 255));
#else
		GetLayer3D()->SetSkyAmbientColor(ace::Color(0, 0, 0, 255));
		GetLayer3D()->SetGroundAmbientColor(ace::Color(0, 0, 0, 255));
#endif

		SetCameraParameter(10, 15, -15, 1, 200, 20);
	}
};

//ENGINE_TEST(Graphics, TerrainObject3D)