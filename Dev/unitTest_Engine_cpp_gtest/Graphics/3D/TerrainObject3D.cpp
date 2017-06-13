
#include "../../EngineGraphics3DTest.h"

class Graphics_TerrainObject3D : public EngineGraphics3DTest
{
public:

	Graphics_TerrainObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("TerrainObject3D"), isOpenGLMode, 15, true)
	{}

protected:
	void OnStart() override
	{
		asd::RenderSettings settings;
		settings.IsLightweightMode = false;
		//settings.VisualizedBuffer = asd::VisualizedBufferType::Smoothness;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		auto graphics = asd::Engine::GetGraphics();

		// 地形
		auto t = graphics->CreateTerrain3D();
		
		t->New(1, 4, 4);

		t->AddSurface(
			asd::ToAString("sf1").c_str(), 
			2,
			asd::ToAString("Data/Texture/Surface/Tile_Color.png").c_str(), 
			asd::ToAString("Data/Texture/Surface/Tile_Normal.png").c_str(),
			asd::ToAString("Data/Texture/Surface/Tile_Spec.png").c_str());

		t->AddSurface(
			asd::ToAString("sf2").c_str(),
			2,
			asd::ToAString("Data/Texture/Surface/Cloud_Color.png").c_str(),
			asd::ToAString("Data/Texture/Surface/Cloud_Normal.png").c_str(),
			asd::ToAString("Data/Texture/Surface/Cloud_Spec.png").c_str());

		auto sf2ind = t->GetSurfaceIndex(asd::ToAString("sf2").c_str());
		t->AssignSurfaceWithCircle(sf2ind, 0, 0, 2, 255, 0.5);
		//t->RaiseHeightWithCircle(0, 0, 3.0f, 0.5f, 0.5f);
		//t->ChangeCliffesWithCircle(1, 0, 3.0f, 2);
		//t->ChangeCliffesWithCircle(0, 0, 1.5f, 1);
		
		int heights [] =
		{
			2, 2, 2, 2,
			2, 2, 2, 2,
			1, 1, 0, 0,
			0, 0, 0, 0,
		};

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				t->ChangeCliffesWithCircle(-2 + x + 0.5f, -2 + y + 0.5f, 1.0f, heights[x + y * 4]);
			}
		}

		auto tObj = std::make_shared<asd::TerrainObject3D>();
		tObj->SetTerrain(t);
		GetLayer3D()->AddObject(tObj);

		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		lightObj->SetRotation(asd::Vector3DF(30, 160, 0));
		lightObj->SetColor(asd::Color(255, 255, 255, 200));
		GetLayer3D()->AddObject(lightObj);

		// Environment (Because file is hevey, if you require Cubemap, you need to put a file manually.)
		auto cubemap = asd::Engine::GetGraphics()->CreateCubemapTexture(u"env.dds");
		auto specCubemap = asd::Engine::GetGraphics()->CreateCubemapTexture(u"spec.dds");
		GetLayer3D()->SetEnvironmentColor(cubemap, specCubemap);

		// 環境光
#if 0
		GetLayer3D()->SetSkyAmbientColor(asd::Color(10, 10, 20, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(20, 10, 10, 255));
#else
		GetLayer3D()->SetSkyAmbientColor(asd::Color(0, 0, 0, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(0, 0, 0, 255));
#endif

		SetCameraParameter(10, 15, -15, 1, 200, 20);
	}
};

ENGINE_TEST(Graphics, TerrainObject3D)