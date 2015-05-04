
#include "../../EngineGraphics3DTest.h"

static const char code[] =  R"(

Texture2D		g_colorTexture : register(t0);
SamplerState	g_colorSampler : register(s0);

Texture2D		g_normalTexture : register(t1);
SamplerState	g_normalSampler : register(s1);

Texture2D		g_metalnessTexture : register(t2);
SamplerState	g_metalnessSampler : register(s2);

Texture2D		g_smoothnessTexture : register(t3);
SamplerState	g_smoothnessSampler : register(s3);

Texture2D		g_aoTexture : register(t4);
SamplerState	g_aoSampler : register(s4);

float4			colorIntensity;

#ifdef LIGHTWEIGHT
struct PS_Input
{
	float4 SV_Position : SV_POSITION;

	float4 Position : POSITION0;
	float4 Color : Color0;
	half2 UV : TEXCOORD0;
	half3 Normal : NORMAL0;
};

#else
struct PS_Input
{
	float4 SV_Position : SV_POSITION;

	float4 Position : POSITION0;
	float4 ProjPosition : PROJPOSITION0;
	float4 Color : Color0;
	half2 UV : UV0;
	half2 UVSub : UVSub0;
	half3 Normal : NORMAL0;
	half3 Binormal : BINORMAL0;
	half3 Tangent : TANGENT0;
};

#endif

#ifdef EXPORT_DEPTH
struct PS_Output
{
	float4 Depth : SV_Target0;
};
#else
#ifdef LIGHTWEIGHT
struct PS_Output
{
	float4 DiffuseColor : SV_Target0;
};
#else
struct PS_Output
{
	float4 DiffuseColor : SV_Target0;
	float4 SmoothnessMetalnessAO : SV_Target1;
	float4 NormalDepth : SV_Target2;
	float4 AO_MatID : SV_Target3;
};
#endif
#endif

#ifndef LIGHTWEIGHT
half3 CalculateNormal(half3 normal, half3 tangent, half3 binormal, float3 normalMap)
{
	half3 n = (normalMap - 0.5) * 2.0;
	return tangent * n.x + binormal * n.y + normal * n.z;
}
#endif

PS_Output main(const PS_Input Input)
{
	float4 diffuseColor = Input.Color;
	if (diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * g_colorTexture.Sample(g_colorSampler, Input.UV);
	if (diffuseColor.a == 0.0f) discard;

	//diffuseColor.xyz = diffuseColor.xyz * 1.4;

	PS_Output Output;

#ifdef LIGHTWEIGHT
	float projDepth = 0.0;
#else
	float projDepth = (Input.ProjPosition.z / Input.ProjPosition.w);
#endif

#ifdef EXPORT_DEPTH
	// VSM用二乗バッファ含む

	float dx = ddx(projDepth);
	float dy = ddy(projDepth);
	Output.Depth.x = projDepth;
	Output.Depth.y = projDepth * projDepth + 0.25*(dx*dx + dy*dy);
	Output.Depth.z = 0.0;
	Output.Depth.w = projDepth;
#else
	Output.DiffuseColor = diffuseColor;

#ifndef LIGHTWEIGHT

	float metalness = 0.2;
	float smoothness = 0.2;

	Output.DiffuseColor = diffuseColor;

	Output.NormalDepth.xyz = CalculateNormal(Input.Normal, Input.Tangent, Input.Binormal, float3(0.5, 0.5, 1));
	Output.NormalDepth.w = projDepth;

	//Output.NormalDepth.xy = Input.UV.xy;
	//Output.NormalDepth.z = 0.0;

	Output.SmoothnessMetalnessAO.x = smoothness;
	Output.SmoothnessMetalnessAO.y = metalness;
	Output.SmoothnessMetalnessAO.z = 1.0;
	Output.SmoothnessMetalnessAO.w = 1.0;

	Output.AO_MatID.x = 1.0;
	Output.AO_MatID.y = 0;
	Output.AO_MatID.z = 0;
	Output.AO_MatID.w = 0;
#endif
#endif

	return Output;
}
)";
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

		auto cubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFrom6ImageFiles(
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Front.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Left.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Back.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Right.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Top.png").c_str(),
			ace::ToAString("Data/Cubemap/Sky1/Diffuse/Bottom.png").c_str()
			);

		auto specCubemap = ace::Engine::GetGraphics()->CreateCubemapTextureFromMipmapImageFiles(ace::ToAString("Data/Cubemap/Sky1/Spec/sky").c_str(), 8);

		auto plainObj = std::make_shared<ace::ModelObject3D>();
		auto sphereObj = std::make_shared<ace::ModelObject3D>();
		auto lightObj = std::make_shared<ace::DirectionalLightObject3D>();

		GetLayer3D()->AddObject(plainObj);
		GetLayer3D()->AddObject(sphereObj);
		GetLayer3D()->AddObject(lightObj);

		auto plainModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Plain1.mdl").c_str());
		auto sphereModel = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("Data/Model/Sphere1.mdl").c_str());

		
		auto shader = ace::Engine::GetGraphics()->CreateShader3D(ace::ToAString(code).c_str());
		auto mat = ace::Engine::GetGraphics()->CreateMaterial3D(shader);

		auto reimu = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("REIMU.mdl").c_str());
		auto wf = ace::Engine::GetGraphics()->CreateModel(ace::ToAString("WING_FAIRLY.mdl").c_str());
		auto reimuObj = std::make_shared<ace::ModelObject3D>();
		auto wfObj = std::make_shared<ace::ModelObject3D>();
		reimuObj->SetModel(reimu);
		wfObj->SetModel(wf);

		reimuObj->SetPosition(ace::Vector3DF(-10, 0, 0));
		wfObj->SetPosition(ace::Vector3DF(10, 0, 0));

		reimuObj->PlayAnimation(0, ace::ToAString("IDLE").c_str());
		wfObj->PlayAnimation(0, ace::ToAString("IDLE").c_str());

		auto blockReimu = ace::Engine::GetGraphics()->CreateMaterialPropertyBlock();
		blockReimu->SetTexture2D(ace::ToAString("g_colorTexture").c_str(), ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("REIMU.png").c_str()));
		auto blockWf = ace::Engine::GetGraphics()->CreateMaterialPropertyBlock();
		blockWf->SetTexture2D(ace::ToAString("g_colorTexture").c_str(), ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("WING_FAIRLY.png").c_str()));

		reimu->GetMesh(0)->SetMaterial(0,mat);
		wf->GetMesh(0)->SetMaterial(0,mat);
		wf->GetMesh(0)->SetMaterial(1, mat);

		reimuObj->SetMaterialPropertyBlock(0, 0, blockReimu);
		wfObj->SetMaterialPropertyBlock(0, 0, blockWf);
		wfObj->SetMaterialPropertyBlock(0, 1, blockWf);
		wfObj->SetMaterialPropertyBlock(1, 0, blockWf);
		wfObj->SetMaterialPropertyBlock(1, 1, blockWf);

		ace::Engine::AddObject3D(reimuObj);
		ace::Engine::AddObject3D(wfObj);
		

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
		lightObj->SetIntensity(1);

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
		bloom->SetIntensity(3.0f);
		bloom->SetThreshold(1.0f);
		bloom->SetExposure(1.0f);
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