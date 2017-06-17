
#include "../../EngineGraphics3DTest.h"


class Graphics_LightingStandard : public EngineGraphics3DTest
{
public:

	Graphics_LightingStandard(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("LightingStandard"), isOpenGLMode, 15, true)
	{}

protected:

	std::shared_ptr<asd::Texture2D> luTexs[5];
	std::shared_ptr<asd::Mesh> mainMesh;

	void OnStart() override
	{
		asd::RenderSettings settings;
		//settings.IsLightweightMode = true;
		settings.VisualizedBuffer = asd::VisualizedBufferType::FinalImage;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		// リソース
		luTexs[0] = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/White.png").c_str());
		luTexs[1] = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/LightGray.png").c_str());
		luTexs[2] = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/Gray.png").c_str());
		luTexs[3] = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/DarkGray.png").c_str());
		luTexs[4] = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/Black.png").c_str());

		auto plainObj = std::make_shared<asd::ModelObject3D>();
		auto sphereObj = std::make_shared<asd::ModelObject3D>();

		GetLayer3D()->AddObject(plainObj);
		GetLayer3D()->AddObject(sphereObj);
		
		auto plainModel = asd::Engine::GetGraphics()->CreateModel(asd::ToAString("Data/Model/Plain1.mdl").c_str());
		auto sphereModel = asd::Engine::GetGraphics()->CreateModel(asd::ToAString("Data/Model/Sphere1.mdl").c_str());

		plainObj->SetModel(plainModel);
		sphereObj->SetModel(sphereModel);
		sphereObj->SetPosition(asd::Vector3DF(0.0f, 1.0f, 0.0f));
		
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		GetLayer3D()->AddObject(lightObj);


		SetCameraParameter(10, 15, -15, 1, 200, 20);

		//mainMesh = sphereModel->GetMesh(0);

		// 球素材
		//auto mesh = sphereModel->GetMesh(0);
		//mesh->SetColorTexture(0,luTexs[0]);
		//mesh->SetMetalnessTexture(0, luTexs[2]);
		//mesh->SetSmoothnessTexture(0, luTexs[2]);

		// 直接光
		lightObj->SetRotation(asd::Vector3DF(30, 140, 0));
		lightObj->SetColor(asd::Color(255, 255, 255, 200));
		lightObj->SetIntensity(1);

		// Environment (Because file is hevey, if you require Cubemap, you need to put a file manually.)
		auto cubemap = asd::Engine::GetGraphics()->CreateCubemapTexture(u"env.dds");
		auto specCubemap = asd::Engine::GetGraphics()->CreateCubemapTexture(u"spec.dds");
		GetLayer3D()->SetEnvironmentColor(cubemap, specCubemap);
		GetLayer3D()->SetEnvironmentColorIntensity(0.5, 0.5);

#if 0
		GetLayer3D()->SetSkyAmbientColor(asd::Color(10, 10, 20, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(20, 10, 10, 255));
#else
		GetLayer3D()->SetSkyAmbientColor(asd::Color(0, 0, 0, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(0, 0, 0, 255));
#endif

#if 1
		auto bloom = std::make_shared<asd::PostEffectLightBloom>();
		bloom->SetIntensity(3.0f);
		bloom->SetThreshold(1.0f);
		bloom->SetExposure(1.0f);
		GetLayer3D()->AddPostEffect(bloom);
#endif
	}

	void OnUpdating()
	{
		EngineGraphics3DTest::OnUpdating();

		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Q) == asd::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[0]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::W) == asd::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[1]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::E) == asd::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[2]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::R) == asd::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[3]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::T) == asd::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[4]);

		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::A) == asd::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[0]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::S) == asd::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[1]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::D) == asd::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[2]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::F) == asd::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[3]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::G) == asd::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[4]);

		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Z) == asd::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[0]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::X) == asd::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[1]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::C) == asd::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[2]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::V) == asd::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[3]);
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::B) == asd::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[4]);

	}
};

ENGINE_TEST(Graphics, LightingStandard)