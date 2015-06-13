
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
	float4 SmoothnessMetalnessAO		: SV_Target1;
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

	Output.SmoothnessMetalnessAO.xyz = float3(0.0,0.0,1.0);
	Output.SmoothnessMetalnessAO.w = 0.0;

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
layout(location = 1) out vec4 SmoothnessMetalnessAO;
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

	SmoothnessMetalnessAO.xyz = vec3(0.0,0.0,1.0);
	SmoothnessMetalnessAO.w = 0.0;

	AO_MatID.x = 1.0;
	AO_MatID.y = 0;
	AO_MatID.z = 0;
	AO_MatID.w = 0;
#endif
#endif
}

)";

class Graphics_MassModelObject3D : public EngineGraphics3DTest
{

public:

	Graphics_MassModelObject3D(bool isOpenGLMode) :
		EngineGraphics3DTest(asd::ToAString("MassModelObject3D"), isOpenGLMode, 15, true)
	{}

protected:
	std::vector<std::shared_ptr<asd::MassModelObject3D>> meshObjs;


	void OnStart() override
	{
		asd::RenderSettings settings;
		settings.IsLightweightMode = false;
		SetRenderSettings(settings);

		EngineGraphics3DTest::OnStart();


		auto graphics = asd::Engine::GetGraphics();

		auto massModel = graphics->CreateMassModelFromModelFile(asd::ToAString("Data/Model/MassTest1.mdl").c_str());
		
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

		auto gray = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Model/Texture/Gray.png").c_str());

		auto block1 = asd::Engine::GetGraphics()->CreateMaterialPropertyBlock();
		block1->SetTexture2D(asd::ToAString("g_colorTexture").c_str(), gray);
		block1->SetVector4DF(asd::ToAString("extColor").c_str(), asd::Vector4DF(1.0f, 1.0f, 0.0f, 1.0f));

		auto block2 = asd::Engine::GetGraphics()->CreateMaterialPropertyBlock();
		block2->SetTexture2D(asd::ToAString("g_colorTexture").c_str(), gray);
		block2->SetVector4DF(asd::ToAString("extColor").c_str(), asd::Vector4DF(1.0f, 1.0f, 1.0f, 1.0f));

		massModel->SetMaterial(mat);

		for (int32_t i = -2; i <= 2; i++)
		{
			auto m = std::make_shared<asd::MassModelObject3D>();
			m->SetModel(massModel);

			m->SetPosition(asd::Vector3DF(3 * i, 0, 0));
			GetLayer3D()->AddObject(m);

			meshObjs.push_back(m);

			if (i == 0) m->PlayAnimation(asd::ToAString("Anime1").c_str());
			if (i == 1) m->PlayAnimation(asd::ToAString("Anime2").c_str());

			if (i == -1)
			{
				m->SetMaterialPropertyBlock(block1);
			}
			else
			{
				m->SetMaterialPropertyBlock(block2);
			}
		}
		
		auto lightObj = std::make_shared<asd::DirectionalLightObject3D>();
		lightObj->SetRotation(asd::Vector3DF(120, 50, 0));
		GetLayer3D()->AddObject(lightObj);

		SetCameraParameter(30, 0, 0, 1, 60, 20);
		GetLayer3D()->SetSkyAmbientColor(asd::Color(50, 50, 70, 255));
		GetLayer3D()->SetGroundAmbientColor(asd::Color(70, 70, 50, 255));
		
	}

	void OnUpdating() override
	{
		EngineGraphics3DTest::OnUpdating();
	}
};

ENGINE_TEST(Graphics, MassModelObject3D)