
#include "../../EngineGraphics3DTest.h"


class Graphics_LightingStandard : public EngineGraphics3DTest
{
public:

	Graphics_LightingStandard(bool isOpenGLMode) :
		EngineGraphics3DTest(ace::ToAString("LightingStandard"), isOpenGLMode, 15000, true)
	{}

protected:

	std::shared_ptr<ace::Texture2D> luTexs[5];
	std::shared_ptr<ace::Mesh> mainMesh;

	void OnStart() override
	{
		ace::RenderSettings settings;
		//settings.IsLightweightMode = true;
		settings.VisualizedBuffer = ace::VisualizedBufferType::FinalImage;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		// リソース
		luTexs[0] = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/White.png").c_str());
		luTexs[1] = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/LightGray.png").c_str());
		luTexs[2] = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/Gray.png").c_str());
		luTexs[3] = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/DarkGray.png").c_str());
		luTexs[4] = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Model/Texture/Black.png").c_str());

		auto cubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFromSingleImageFile(ace::ToAString("Data/Forest_Sunny01/Forest_Sunny01DiffuseHDR.dds").c_str());
		auto specCubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFromSingleImageFile(ace::ToAString("Data/Forest_Sunny01/Forest_Sunny01SpecularHDR.dds").c_str());

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

		SetCameraParameter(10, 15, -15, 1, 200, 20);

		mainMesh = sphereModel->GetMesh(0);

		// 球素材
		auto mesh = sphereModel->GetMesh(0);
		mesh->SetColorTexture(0,luTexs[0]);
		mesh->SetMetalnessTexture(0, luTexs[2]);
		mesh->SetSmoothnessTexture(0, luTexs[2]);

		// 直接光
		lightObj->SetRotation(ace::Vector3DF(30, 140, 0));
		lightObj->SetColor(ace::Color(255, 255, 255, 200));
		lightObj->SetIntensity(0);

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

#if 1
		auto bloom = std::make_shared<ace::PostEffectLightBloom>();
		bloom->SetIntensity(8.0f);
		bloom->SetThreshold(1.0f);
		bloom->SetPower(1.0f);
		GetLayer3D()->AddPostEffect(bloom);
#endif
	}

	void OnUpdating()
	{
		EngineGraphics3DTest::OnUpdating();

		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Q) == ace::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[0]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::W) == ace::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[1]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::E) == ace::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[2]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::R) == ace::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[3]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::T) == ace::KeyState::Push) mainMesh->SetColorTexture(0, luTexs[4]);

		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::A) == ace::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[0]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::S) == ace::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[1]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::D) == ace::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[2]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::F) == ace::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[3]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::G) == ace::KeyState::Push) mainMesh->SetMetalnessTexture(0, luTexs[4]);

		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z) == ace::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[0]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::X) == ace::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[1]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::C) == ace::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[2]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::V) == ace::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[3]);
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::B) == ace::KeyState::Push) mainMesh->SetSmoothnessTexture(0, luTexs[4]);

	}
};

ENGINE_TEST(Graphics, LightingStandard)