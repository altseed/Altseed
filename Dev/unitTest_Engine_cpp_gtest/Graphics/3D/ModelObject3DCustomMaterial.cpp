
#include "../../EngineGraphics3DTest.h"

static const char* shader3d_dx_ps = R"(

Texture2D		g_colorTexture;
SamplerState	g_colorSampler;

#ifdef LIGHTWEIGHT
struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 Color			: Color0;
	half2 UV				: UV0;
	half3 Normal			: NORMAL0;
};

#else
struct PS_Input
{
	float4 SV_Position		: SV_POSITION;

	float4 Position			: POSITION0;
	float4 ProjPosition		: PROJPOSITION0;
	float4 Color			: Color0;
	half2 UV				: UV0;
	half2 UVSub				: UVSub0;
	half3 Normal			: NORMAL0;
	half3 Binormal			: BINORMAL0;
	half3 Tangent			: TANGENT0;
};

#endif

#ifdef EXPORT_DEPTH
struct PS_Output
{
	float4 Depth					: SV_Target0;
};
#else
#ifdef LIGHTWEIGHT
struct PS_Output
{
	float4 DiffuseColor					: SV_Target0;
};
#else
struct PS_Output
{
	float4 DiffuseColor					: SV_Target0;
	float4 SpecularColor_Smoothness		: SV_Target1;
	float4 NormalDepth					: SV_Target2;
	float4 AO_MatID						: SV_Target3;
};
#endif
#endif

float4 extColor;

PS_Output main( const PS_Input Input )
{
	float4 diffuseColor = Input.Color;
	if(diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * g_colorTexture.Sample(g_colorSampler, Input.UV);
	if(diffuseColor.a == 0.0f) discard;	

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
	Output.Depth.y = projDepth * projDepth + 0.25*(dx*dx+dy*dy);
	Output.Depth.z = 0.0;
	Output.Depth.w = projDepth;
#else
	Output.DiffuseColor = diffuseColor * extColor;

#ifndef LIGHTWEIGHT
	Output.NormalDepth.xyz = Input.Normal;
	Output.NormalDepth.w = projDepth;

	Output.SpecularColor_Smoothness.xyz = float3(0.0,0.0,0.0);
	Output.SpecularColor_Smoothness.w = 0.0;

	Output.AO_MatID.x = 1.0;
	Output.AO_MatID.y = 0;
	Output.AO_MatID.z = 0;
	Output.AO_MatID.w = 0;
#endif
#endif

	return Output;
}

)";

static const char* shader3d_gl_ps = R"(
uniform sampler2D g_colorTexture;

#ifdef LIGHTWEIGHT
in vec4 voutPosition;
in vec4 voutColor;
in vec2 voutUV;
in vec3 voutNormal;
#else
in vec4 voutPosition;
in vec4 voutProjPosition;
in vec4 voutColor;
in vec2 voutUV;
in vec2 voutUVSub;
in vec3 voutNormal;
in vec3 voutBinormal;
in vec3 voutTangent;
#endif

#ifdef EXPORT_DEPTH
out vec4 Depth;
#else
#ifdef LIGHTWEIGHT
layout(location = 0) out vec4 DiffuseColor;
#else
layout(location = 0) out vec4 DiffuseColor;
layout(location = 1) out vec4 SpecularColor_Smoothness;
layout(location = 2) out vec4 NormalDepth;
layout(location = 3) out vec4 AO_MatID;
#endif
#endif

uniform vec4 extColor;

void main()
{
	vec2 voutUV_ = voutUV;
	voutUV_.y = 1.0 - voutUV_.y;

	vec4 diffuseColor = voutColor;
	if(diffuseColor.a == 0.0f) discard;

	diffuseColor = diffuseColor * texture(g_colorTexture, voutUV_);
	if(diffuseColor.a == 0.0f) discard;	

#ifdef LIGHTWEIGHT
	float projDepth = 0.0;
#else
	float projDepth = (voutProjPosition.z / voutProjPosition.w);
#endif

	// GL
	projDepth = projDepth * 0.5 + 0.5;

#ifdef EXPORT_DEPTH
	// VSM用二乗バッファ含む

	float dx = dFdx(projDepth);
	float dy = dFdy(projDepth);
	Depth.x = projDepth;
	Depth.y = projDepth * projDepth + 0.25*(dx*dx+dy*dy);
	Depth.z = 0.0;
	Depth.w = projDepth;
#else

	DiffuseColor = diffuseColor * extColor;

#ifndef LIGHTWEIGHT
	NormalDepth.xyz = voutNormal;
	NormalDepth.w = projDepth;

	SpecularColor_Smoothness.xyz = vec3(0.0,0.0,0.0);
	SpecularColor_Smoothness.w = 0.0;

	AO_MatID.x = 1.0;
	AO_MatID.y = 0;
	AO_MatID.z = 0;
	AO_MatID.w = 0;
#endif
#endif
}

)";

class Graphics_ModelObject3DCustomMaterial : public EngineGraphics3DTest
{
public:

	Graphics_ModelObject3DCustomMaterial(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("ModelObject3DCustomMaterial"), isOpenGLMode, 15, true)
	{}

protected:
	void OnStart() override
	{
		asd::RenderSettings settings;
		//settings.IsLightweightMode = true;
		//settings.VisualizedBuffer = asd::VisualizedBufferType::Normal;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();

		// リソース
		auto gray = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/Gray.png").c_str());

		auto sphereObj = std::make_shared<asd::ModelObject3D>();
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();

		GetLayer3D()->AddObject(sphereObj);
		GetLayer3D()->AddObject(lightObj);

		auto sphereModel = asd::Engine::GetGraphics()->CreateModel(asd::ToAString("Data/Model/Sphere1.mdl").c_str());

		sphereObj->SetModel(sphereModel);
		sphereObj->SetPosition(asd::Vector3DF(0.0f, 1.0f, 0.0f));

		SetCameraParameter(10, 15, -15, 1, 200, 20);

		auto mainMesh = sphereModel->GetMesh(0);

		// 球素材
		std::shared_ptr<asd::Shader3D> matShader;
		
		if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::DirectX11)
		{
			matShader = asd::Engine::GetGraphics()->CreateShader3D(asd::ToAString(shader3d_dx_ps).c_str());
		}
		else if (asd::Engine::GetGraphics()->GetGraphicsDeviceType() == asd::GraphicsDeviceType::OpenGL)
		{
			matShader = asd::Engine::GetGraphics()->CreateShader3D(asd::ToAString(shader3d_gl_ps).c_str());
		}

		auto mat = asd::Engine::GetGraphics()->CreateMaterial3D(matShader);

		auto mesh = sphereModel->GetMesh(0);
		mesh->SetMaterial(0, mat);

		auto block = asd::Engine::GetGraphics()->CreateMaterialPropertyBlock();
		block->SetTexture2D(asd::ToAString("g_colorTexture").c_str(), gray);
		block->SetVector4DF(asd::ToAString("extColor").c_str(), asd::Vector4DF(1.0f, 1.0f, 0.0f, 1.0f));
		sphereObj->SetMaterialPropertyBlock(0, 0, block);

		// 直接光
		lightObj->SetRotation(asd::Vector3DF(30, 160, 0));
		lightObj->SetColor(asd::Color(255 / 1, 255 / 1, 255 / 1, 200));

		// 環境光
		GetLayer3D()->SetSkyAmbientColor(asd::Color(10, 10, 20, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(20, 10, 10, 255));

	}
};

ENGINE_TEST(Graphics, ModelObject3DCustomMaterial)