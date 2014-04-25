
#include "ace.RenderedModelObject3D.h"
#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"
#include "ace.Model_Imp.h"

#include "../Animation/ace.AnimationClip_Imp.h"
#include "../Animation/ace.AnimationSource_Imp.h"
#include "../Animation/ace.KeyframeAnimation_Imp.h"

#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"

namespace ace
{
	static const char* dx_vs = R"(

float4x4	matM[32]					: register( c0 );
float4x4	matC						: register( c128 );
float4x4	matP						: register( c132 );
float4x4	matLCP						: register( c136 );

float3		directionalLightDirection	: register( c140 );
float3		directionalLightColor		: register( c141 );

struct VS_Input
{
	float3 Position		: Position0;
	float3 Normal		: Normal0;
	float3 Binormal		: Binormal0;
	float2 UV			: UV0;
	float2 UVSub		: UVSub0;
	float4 Color		: Color0;
	float4 BoneWeights	: BoneWeights0;
	uint4 BoneIndexes	: BoneIndexes0;
	uint4 BoneIndexesOriginal	: BoneIndexesOriginal0;
};

struct VS_Output
{
	float4 Position		: SV_POSITION;
	float4 Pos			: POSITION0;
	float4 LightPos	: Light0;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
	half3 Normal	: NORMAL0;
};

float4x4 calcMatrix(float4 weights, uint4 indexes)
{
	return matM[indexes.x] * weights.x +
	matM[indexes.y] * weights.y +
	matM[indexes.z] * weights.z +
	matM[indexes.w] * weights.w;
}

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	//Output.Pos = mul( matMCP[0], float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );

	float4x4 matLocal = calcMatrix(Input.BoneWeights,Input.BoneIndexes);
	float4x4 matMC = mul(matC, matLocal);
	float4x4 matMLCP = mul(matLCP, matLocal);

	float4 cPosition = mul( matMC, float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );
	cPosition = cPosition / cPosition.w;

	float4 cNormal = mul( matMC, float4( Input.Normal.x + Input.Position.x, Input.Normal.y + Input.Position.y, Input.Normal.z + Input.Position.z, 1.0 ) );
	cNormal = cNormal / cNormal.w;

	cNormal = cNormal - cPosition;
	cNormal.xyz = normalize(cNormal.xyz);

	float3 cDirectionalLightDirection = mul(matC, directionalLightDirection).xyz;

	Output.Position = mul( matP, cPosition );
	Output.Pos = Output.Position;
	Output.LightPos = mul( matMLCP, float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ) );

	Output.Normal = (half3)cNormal.xyz;
	Output.UV = Input.UV;
	Output.Color.xyz = directionalLightColor * max( dot(cDirectionalLightDirection.xyz,cNormal.xyz), 0.0 ) + 0.2;
	Output.Color.w = 1.0;
	return Output;
}

)";

static const char* dx_ps = R"(

struct PS_Input
{
	float4 Position		: SV_POSITION;
	float4 Pos			: POSITION0;
	float4 LightPos	: Light0;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
	half3 Normal	: NORMAL0;
};

float4		hasTextures	: register( c0 );

Texture2D		g_colorTexture		: register( t0 );
SamplerState	g_colorSampler		: register( s0 );

Texture2D		g_normalTexture		: register( t1 );
SamplerState	g_normalSampler		: register( s1 );

Texture2D		g_specularTexture		: register( t2 );
SamplerState	g_specularSampler		: register( s2 );

Texture2D		g_shadowTexture		: register( t3 );
SamplerState	g_shadowSampler		: register( s3 );

Texture2D		g_ssaoTexture		: register( t4 );
SamplerState	g_ssaoSampler		: register( s4 );

float VSM(float2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;

	float p = (t <= ex);
	float variance = ex2 - ex * ex;
	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}

float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;

	if(hasTextures.x != 0.0)
	{
		Output = Output * g_colorTexture.Sample(g_colorSampler, Input.UV);
	}
	if(Output.a == 0.0f) discard;

	// SSAO
	float2 ssaoUV = float2( (Input.Pos.x / Input.Pos.w + 1.0) / 2.0, (-Input.Pos.y / Input.Pos.w + 1.0) / 2.0 );
	float a = g_ssaoTexture.Sample(g_ssaoSampler, ssaoUV).x;

	// shadow
	float2 shadowUV = float2( (Input.LightPos.x / Input.LightPos.w + 1.0) / 2.0, 1.0 - (Input.LightPos.y / Input.LightPos.w + 1.0) / 2.0 );
	float lightDepthZ = Input.LightPos.z / Input.LightPos.w;

	float2 shadowParam = g_shadowTexture.Sample(g_shadowSampler, shadowUV).xy;

	float p = VSM(shadowParam, lightDepthZ - 0.02 );

	//float bias = 0.01;
	//if(shadowZ > g_shadowTexture.Sample(g_shadowSampler, shadowUV).r + bias )
	//{
	//	Output.rgb = Output.rgb * 0.5;
	//}

	Output.rgb = Output.rgb * p * a;
	return Output;
}

)";

static const char* dx_shadow_ps = R"(

struct PS_Input
{
	float4 Position		: SV_POSITION;
	float4 Pos			: POSITION0;
	float4 LightPos	: Light0;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
	half3 Normal	: NORMAL0;
};

float4		hasTextures	: register( c0 );

Texture2D		g_colorTexture		: register( t0 );
SamplerState	g_colorSampler		: register( s0 );

Texture2D		g_normalTexture		: register( t1 );
SamplerState	g_normalSampler		: register( s1 );

Texture2D		g_specularTexture		: register( t2 );
SamplerState	g_specularSampler		: register( s2 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;

	if(hasTextures.x != 0.0)
	{
		Output = Output * g_colorTexture.Sample(g_colorSampler, Input.UV);
	}
	if(Output.a == 0.0f) discard;
	
	float color = Input.Pos.z / Input.Pos.w;
	return float4( color, color * color, 0, 1 );
}

)";

static const char* dx_normal_depth_ps = R"(

struct PS_Input
{
	float4 Position		: SV_POSITION;
	float4 Pos			: POSITION0;
	float4 LightPos	: Light0;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
	half3 Normal	: NORMAL0;
};

float4		hasTextures	: register( c0 );

Texture2D		g_colorTexture		: register( t0 );
SamplerState	g_colorSampler		: register( s0 );

Texture2D		g_normalTexture		: register( t1 );
SamplerState	g_normalSampler		: register( s1 );

Texture2D		g_specularTexture		: register( t2 );
SamplerState	g_specularSampler		: register( s2 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;

	if(hasTextures.x != 0.0)
	{
		Output = Output * g_colorTexture.Sample(g_colorSampler, Input.UV);
	}
	if(Output.a == 0.0f) discard;
	
	float depth = Input.Pos.z / Input.Pos.w;
	return float4( (Input.Normal.x + 1.0) / 2.0, (Input.Normal.y + 1.0) / 2.0, (Input.Normal.z + 1.0) / 2.0, depth );
}

)";

static const char* gl_vs = R"(

attribute vec3 Position;
attribute vec3 Normal;
attribute vec3 Binormal;
attribute vec2 UV;
attribute vec2 UVSub;
attribute vec4 Color;
attribute vec4 BoneWeights;
attribute vec4 BoneIndexes;
attribute vec4 BoneIndexesOriginal;

uniform mat4		matM[32];
uniform mat4		matC;
uniform mat4		matP;
uniform mat4		matLCP;

uniform vec3		directionalLightDirection;
uniform vec3		directionalLightColor;

varying vec4 vaPos;
varying vec4 vaLightPos;
varying vec2 vaUV;
varying vec4 vaColor;
varying vec3 vaNormal;

mat4 calcMatrix(vec4 weights, vec4 indexes)
{
	return matM[int(indexes.x)] * weights.x +
	matM[int(indexes.y)] * weights.y +
	matM[int(indexes.z)] * weights.z +
	matM[int(indexes.w)] * weights.w;
}

void main()
{
	//Output.Pos = mul( matMCP[0], vec4( Position.x, Position.y, Position.z, 1.0 ) );

	mat4 matLocal = calcMatrix(BoneWeights,BoneIndexes);
	mat4 matMC = matC * matLocal;
	mat4 matMLCP = matLCP * matLocal;

	vec4 cPosition = matMC * vec4( Position.x, Position.y, Position.z, 1.0 );
	cPosition = cPosition / cPosition.w;

	vec4 cNormal = matMC * vec4( Normal.x + Position.x, Normal.y + Position.y, Normal.z + Position.z, 1.0 );
	cNormal = cNormal / cNormal.w;

	cNormal = cNormal - cPosition;
	cNormal.xyz = normalize(cNormal).xyz;

	vec3 cDirectionalLightDirection = (matC * vec4(directionalLightDirection, 0.0) ).xyz;

	vaPos = matP * cPosition;
	vaLightPos = matMLCP * vec4( Position.x, Position.y, Position.z, 1.0 );

	vaNormal = cNormal.xyz;
	vaUV = UV;
	vaColor.xyz = directionalLightColor * max( dot(cDirectionalLightDirection.xyz,cNormal.xyz), 0.0 ) + 0.2;
	vaColor.w = 1.0;

	gl_Position = vaPos;
}

)";

static const char* gl_ps = R"(

varying vec4 vaPos;
varying vec4 vaLightPos;
varying vec2 vaUV;
varying vec4 vaColor;
varying vec3 vaNormal;

uniform vec4 hasTextures;

uniform sampler2D g_colorTexture;
uniform sampler2D g_normalTexture;
uniform sampler2D g_specularTexture;
uniform sampler2D g_shadowTexture;

float VSM(vec2 moments, float t)
{
	float ex = moments.x;
	float ex2 = moments.y;

	float p = 0.0;
	if (t <= ex)
	{
		p = 1.0;
	}

	float variance = ex2 - ex * ex;
	float d = t - ex;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}

void main() 
{
	gl_FragColor = vaColor;

	if(gl_FragColor.a == 0.0f) discard;

	// gl only
	vec2 vaUV_ = vaUV;
	vaUV_.y = 1.0 - vaUV_.y;

	if(hasTextures.x != 0.0)
	{
		gl_FragColor = gl_FragColor * texture2D(g_colorTexture, vaUV_.xy);
	}

	if(gl_FragColor.a == 0.0f) discard;

	// shadow
	vec2 shadowUV = vec2( (vaLightPos.x / vaLightPos.w + 1.0) / 2.0, (vaLightPos.y / vaLightPos.w + 1.0) / 2.0 );
	float shadowZ = vaLightPos.z / vaLightPos.w;
	shadowZ = (shadowZ + 1.0) / 2.0;

	float p = VSM(texture2D(g_shadowTexture, shadowUV).xy, shadowZ - 0.02 );

	/*
	float bias = 0.01;
	if(shadowZ > texture2D(g_shadowTexture, shadowUV).r + bias )
	{
		gl_FragColor.rgb = gl_FragColor.rgb * 0.5;
	}
	*/

	gl_FragColor.rgb = gl_FragColor.rgb * p;

	//gl_FragColor.r = shadowZ;
	//gl_FragColor.g = texture2D(g_shadowTexture, shadowUV).r;
}

)";

static const char* gl_shadow_ps = R"(
varying vec4 vaPos;
varying vec4 vaLightPos;
varying vec2 vaUV;
varying vec4 vaColor;
varying vec3 vaNormal;

uniform vec4 hasTextures;

uniform sampler2D g_colorTexture;
uniform sampler2D g_normalTexture;
uniform sampler2D g_specularTexture;

void main() 
{
	gl_FragColor = vaColor;

	if(gl_FragColor.a == 0.0f) discard;

	// gl only
	vec2 vaUV_ = vaUV;
	vaUV_.y = 1.0 - vaUV_.y;

	if(hasTextures.x != 0.0)
	{
		gl_FragColor = gl_FragColor * texture2D(g_colorTexture, vaUV_.xy);
	}

	if(gl_FragColor.a == 0.0) discard;

	float color = (vaPos.z / vaPos.w + 1.0) / 2.0;
	gl_FragColor= vec4( color, color * color, 0, 1 );
}

)";

static const char* gl_normal_depth_ps = R"(
varying vec4 vaPos;
varying vec4 vaLightPos;
varying vec2 vaUV;
varying vec4 vaColor;
varying vec3 vaNormal;

uniform vec4 hasTextures;

uniform sampler2D g_colorTexture;
uniform sampler2D g_normalTexture;
uniform sampler2D g_specularTexture;

void main() 
{
	gl_FragColor = vaColor;

	if(gl_FragColor.a == 0.0f) discard;

	// gl only
	vec2 vaUV_ = vaUV;
	vaUV_.y = 1.0 - vaUV_.y;

	if(hasTextures.x != 0.0)
	{
		gl_FragColor = gl_FragColor * texture2D(g_colorTexture, vaUV_.xy);
	}

	if(gl_FragColor.a == 0.0) discard;

	float depth = (vaPos.z / vaPos.w + 1.0) / 2.0;
	gl_FragColor= vec4( (vaNormal.x + 1.0) / 2.0, (vaNormal.y + 1.0) / 2.0, (vaNormal.z + 1.0) / 2.0, depth );
}

)";

	RenderedModelObject3D::BoneProperty::BoneProperty()
	{
		Position[0] = 0.0f;
		Position[1] = 0.0f;
		Position[2] = 0.0f;

		Rotation[0] = 0.0f;
		Rotation[1] = 0.0f;
		Rotation[2] = 0.0f;
		Rotation[3] = 0.0f;

		Scale[0] = 1.0f;
		Scale[1] = 1.0f;
		Scale[2] = 1.0f;
	}


	Matrix44 RenderedModelObject3D::BoneProperty::CalcMatrix(eRotationOrder rotationType)
	{
		return ModelUtils::CalcMatrix(
			Position,
			Rotation,
			Scale,
			rotationType);
	}

	RenderedModelObject3D::MeshGroup::MeshGroup()
		: m_deformer(nullptr)
	{
	}

	RenderedModelObject3D::MeshGroup::~MeshGroup()
	{
		for (auto& mesh : m_meshes)
		{
			mesh->Release();
		}
		m_meshes.clear();

		SafeRelease(m_deformer);
	}

	void RenderedModelObject3D::MeshGroup::Flip(AnimationClip* animationClip, int32_t time)
	{
		CalculateAnimation(animationClip, time);

		CalclateBoneMatrices();

		// コピー
		if (m_matrixes_fr.size() != m_matrixes.size())
		{
			m_matrixes_fr.resize(m_matrixes.size());
		}

		std::copy(m_matrixes.begin(), m_matrixes.end(), m_matrixes_fr.begin());
	}


	void RenderedModelObject3D::MeshGroup::CalculateAnimation(AnimationClip* animationClip, int32_t time)
	{
		if (animationClip == nullptr) return;

		auto source = (AnimationSource_Imp*) animationClip->GetSource().get();
		auto& animations = source->GetAnimations();

		for (auto& a : animations)
		{
			auto a_ = (KeyframeAnimation_Imp*) a;

			auto type = a_->GetTargetType();
			auto axis = a_->GetTargetAxis();
			auto bi = m_deformer->GetBoneIndex(a_->GetTargetName());

			if (bi < 0) continue;
			auto value = a_->GetValue(time);

			ModelUtils::SetBoneValue(
				m_boneProps[bi].Position,
				m_boneProps[bi].Rotation,
				m_boneProps[bi].Scale,
				type,
				axis,
				value);
		}
	}

	void RenderedModelObject3D::MeshGroup::CalclateBoneMatrices()
	{
		if (m_deformer == nullptr) return;

		for (auto i = 0; i < m_deformer->GetBones().size(); i++)
		{
			auto& b = m_deformer->GetBones()[i];
			m_matrixes[i] = m_boneProps[i].CalcMatrix(b.RotationType);
		}

		ModelUtils::CalculateBoneMatrixes(
			m_matrixes,
			m_deformer->GetBones(),
			m_matrixes);
	}

	void RenderedModelObject3D::MeshGroup::AddMesh(Mesh_Imp* mesh)
	{
		SafeAddRef(mesh);
		m_meshes.push_back(mesh);
	}

	void RenderedModelObject3D::MeshGroup::SetDeformer(Deformer_Imp* deformer)
	{
		SafeSubstitute(m_deformer, deformer);

		if (deformer != nullptr)
		{
			m_matrixes.resize(m_deformer->GetBones().size());
			m_boneProps.resize(m_deformer->GetBones().size());

			for (int32_t i = 0; i < m_boneProps.size(); i++)
			{
				m_boneProps[i] = BoneProperty();
			}
		}
		else
		{
			m_matrixes.resize(0);
			m_boneProps.resize(0);
		}
	}

	RenderedModelObject3D::RenderedModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_animationPlaying(nullptr)
		, m_animationTime(0)
	{
		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneWeights", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneIndexes", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));
		vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));

		std::vector<ace::ConstantBufferInformation> constantBuffers_vs;
		constantBuffers_vs.resize(6);
		constantBuffers_vs[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
		constantBuffers_vs[0].Name = std::string("matM");
		constantBuffers_vs[0].Offset = 0;
		constantBuffers_vs[0].Count = 32;

		constantBuffers_vs[1].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
		constantBuffers_vs[1].Name = std::string("matC");
		constantBuffers_vs[1].Offset = sizeof(Matrix44) * 32;

		constantBuffers_vs[2].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
		constantBuffers_vs[2].Name = std::string("matP");
		constantBuffers_vs[2].Offset = sizeof(Matrix44) * 33;

		constantBuffers_vs[3].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
		constantBuffers_vs[3].Name = std::string("matLCP");
		constantBuffers_vs[3].Offset = sizeof(Matrix44) * 34;

		constantBuffers_vs[4].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
		constantBuffers_vs[4].Name = std::string("directionalLightDirection");
		constantBuffers_vs[4].Offset = sizeof(Matrix44) * 35;

		constantBuffers_vs[5].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
		constantBuffers_vs[5].Name = std::string("directionalLightColor");
		constantBuffers_vs[5].Offset = sizeof(Matrix44) * 35 + sizeof(float) * 4;


		std::vector<ace::ConstantBufferInformation> constantBuffers_ps;
		constantBuffers_ps.resize(1);
		constantBuffers_ps[0].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT4;
		constantBuffers_ps[0].Name = std::string("hasTextures");
		constantBuffers_ps[0].Offset = 0;

		{
			std::vector<ace::Macro> macro;
			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shader = GetGraphics()->CreateShader_Imp(
					gl_vs,
					"vs",
					gl_ps,
					"ps",
					vl,
					macro);
			}
			else
			{
				m_shader = GetGraphics()->CreateShader_Imp(
					dx_vs,
					"vs",
					dx_ps,
					"ps",
					vl,
					macro);
			}

			assert(m_shader != nullptr);

			m_shader->CreateVertexConstantBuffer<VertexConstantBuffer>(constantBuffers_vs);
			m_shader->CreatePixelConstantBuffer<PixelConstantBuffer>(constantBuffers_ps);
		}

		{
			std::shared_ptr<ace::NativeShader_Imp> shader;
			std::vector<ace::Macro> macro;
			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shaderShadow = GetGraphics()->CreateShader_Imp(
					gl_vs,
					"shadow_vs",
					gl_shadow_ps,
					"shadow_ps",
					vl,
					macro);
			}
			else
			{
				m_shaderShadow = GetGraphics()->CreateShader_Imp(
					dx_vs,
					"shadow_vs",
					dx_shadow_ps,
					"shadow_ps",
					vl,
					macro);
			}

			assert(m_shaderShadow != nullptr);

			m_shaderShadow->CreateVertexConstantBuffer<VertexConstantBuffer>(constantBuffers_vs);
			m_shaderShadow->CreatePixelConstantBuffer<PixelConstantBuffer>(constantBuffers_ps);
		}

		{
			std::shared_ptr<ace::NativeShader_Imp> shader;
			std::vector<ace::Macro> macro;
			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shaderNormalDepth = GetGraphics()->CreateShader_Imp(
					gl_vs,
					"shadow_vs",
					gl_normal_depth_ps,
					"shadow_ps",
					vl,
					macro);
			}
			else
			{
				m_shaderNormalDepth = GetGraphics()->CreateShader_Imp(
					dx_vs,
					"shadow_vs",
					dx_normal_depth_ps,
					"shadow_ps",
					vl,
					macro);
			}

			assert(m_shaderNormalDepth != nullptr);

			m_shaderNormalDepth->CreateVertexConstantBuffer<VertexConstantBuffer>(constantBuffers_vs);
			m_shaderNormalDepth->CreatePixelConstantBuffer<PixelConstantBuffer>(constantBuffers_ps);
		}
	}

	RenderedModelObject3D::~RenderedModelObject3D()
	{
		m_meshGroups_fr.clear();

		m_meshGroups.clear();

		if (m_model != nullptr)
		{
			m_model->Detach(this);
		}

		SafeRelease(m_model);

		for (auto& a : m_animationClips)
		{
			a.second->Release();
		}
		m_animationClips.clear();
	}

	void RenderedModelObject3D::SetModel(Model* model)
	{
		UnloadModel();

		if (m_model != nullptr)
		{
			m_model->Detach(this);
		}

		SafeRelease(m_model);

		if (model == nullptr) return;
		auto model_ = (Model_Imp*) model;
		m_model = model_;
		SafeAddRef(m_model);

		m_model->Attach(this);

		LoadModel();
	}

	void RenderedModelObject3D::AddMeshGroup()
	{
		m_meshGroups.push_back(std::make_shared<MeshGroup>());
	}

	int32_t RenderedModelObject3D::GetMeshGroupCount()
	{
		return m_meshGroups.size();
	}

	void RenderedModelObject3D::AddMesh(int32_t meshGroupIndex, Mesh* mesh)
	{
		if (meshGroupIndex >= GetMeshGroupCount()) return;
		m_meshGroups[meshGroupIndex]->AddMesh((Mesh_Imp*)mesh);
	}

	void  RenderedModelObject3D::SetDeformer(int32_t meshGroupIndex, Deformer* deformer)
	{
		if (meshGroupIndex >= GetMeshGroupCount()) return;
		m_meshGroups[meshGroupIndex]->SetDeformer((Deformer_Imp*) deformer);
	}

	void RenderedModelObject3D::UnloadModel()
	{
		// 描画中以外のオブジェクトをリセット

		m_meshGroups.clear();

		for (auto& a : m_animationClips)
		{
			a.second->Release();
		}
		m_animationClips.clear();
	}

	void RenderedModelObject3D::LoadModel()
	{
		if (m_model == nullptr) return;

		int32_t index = 0;
		for (auto& mg : m_model->GetMeshGroups())
		{
			AddMeshGroup();

			for (auto& m : mg->Meshes)
			{
				AddMesh(index, m);
			}
			SetDeformer(index, mg->Deformer_);
			index++;
		}

		for (int32_t i = 0; i < m_model->GetAnimationClips().size(); i++)
		{
			AddAnimationClip(m_model->GetAnimationClipNames()[i].c_str(), m_model->GetAnimationClips()[i]);
		}
	}

	void RenderedModelObject3D::ReloadModel()
	{
		UnloadModel();
		LoadModel();
	}

	void RenderedModelObject3D::AddAnimationClip(const achar* name, AnimationClip* animationClip)
	{
		if (animationClip == nullptr) return;

		if (m_animationClips.find(name) == m_animationClips.end())
		{
			SafeAddRef(animationClip);
			m_animationClips[name] = animationClip;
		}
	}

	void RenderedModelObject3D::PlayAnimation(const achar* name)
	{
		auto it = m_animationClips.find(name);
		if (it == m_animationClips.end()) return;

		m_animationPlaying = (*it).second;
		m_animationTime = 0;
	}

	void RenderedModelObject3D::Flip()
	{
		RenderedObject3D::Flip();

		for (auto& g : m_meshGroups)
		{
			g->Flip(m_animationPlaying, m_animationTime);
		}

		// アニメーションの適用
		if (m_animationPlaying != nullptr)
		{
			m_animationTime++;
		}

		m_meshGroups_fr.clear();
		m_meshGroups_fr.resize(m_meshGroups.size());
		std::copy(m_meshGroups.begin(), m_meshGroups.end(), m_meshGroups_fr.begin());
	}

	void RenderedModelObject3D::Rendering(RenderingProperty& prop)
	{
		auto& vbuf = m_shader->GetVertexConstantBuffer<VertexConstantBuffer>();
		auto& pbuf = m_shader->GetPixelConstantBuffer<PixelConstantBuffer>();

		for (auto& g : m_meshGroups_fr)
		{
			auto& matrices = g->m_matrixes_fr;

			for (auto& mesh : g->GetMeshes())
			{
				// 有効チェック
				if (mesh->GetIndexBuffer() == nullptr) continue;

				// 行列計算
				if (matrices.size() > 0)
				{
					// ボーンあり
					for (int32_t i = 0; i < Min(32, matrices.size()); i++)
					{
						vbuf.MMatrices[i].SetIndentity();
						Matrix44::Mul(vbuf.MMatrices[i], GetLocalMatrix_FR(), matrices[i]);
					}
				}
				else
				{
					// ボーンなし
					vbuf.MMatrices[0] = GetLocalMatrix_FR();
					for (int32_t i = 1; i < 32; i++)
					{
						vbuf.MMatrices[i] = vbuf.MMatrices[0];
					}
				}

				{
					vbuf.CMatrix = prop.CameraMatrix;
					vbuf.PMatrix = prop.ProjectionMatrix;
					vbuf.LightVPMatrix = prop.LightProjectionMatrix;
				}

				{
					vbuf.DirectionalLightDirection = prop.DirectionalLightDirection;
					vbuf.DirectionalLightColor.X = prop.DirectionalLightColor.R / 255.0f;
					vbuf.DirectionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f;
					vbuf.DirectionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f;
				}

				auto& boneOffsets = mesh->GetBoneOffsets();
				auto& materialOffsets = mesh->GetMaterialOffsets();

				{
					// 設定がある場合
					auto mIndex = 0;
					auto bIndex = 0;

					auto fOffset = 0;
					auto fCount = 0;

					auto bFCount = 0;
					if (boneOffsets.size() > 0)
					{
						bFCount = boneOffsets[bIndex].FaceOffset;
					}
					else
					{
						bFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					auto mFCount = 0;
					if (materialOffsets.size() > 0)
					{
						mFCount = materialOffsets[mIndex].FaceOffset;
					}
					else
					{
						mFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}
					
					while (fCount < mesh->GetIndexBuffer()->GetCount() / 3)
					{
						fCount = Min(bFCount, mFCount) - fOffset;
						if (fCount == 0) break;

						Mesh_Imp::Material* material = nullptr;
						if (materialOffsets.size() > 0)
						{
							material = mesh->GetMaterial(materialOffsets[mIndex].MaterialIndex);
						}

						if (material != nullptr)
						{
							pbuf.HasTextures.X = material->ColorTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Y = material->NormalTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Z = material->SpecularTexture != nullptr ? 1.0f : 0.0f;

							if (material->ColorTexture != nullptr)
							{
								m_shader->SetTexture("g_colorTexture", material->ColorTexture, 0);
							}

							if (material->NormalTexture != nullptr)
							{
								m_shader->SetTexture("g_normalTexture", material->NormalTexture, 1);
							}

							if (material->SpecularTexture != nullptr)
							{
								m_shader->SetTexture("g_specularTexture", material->SpecularTexture, 2);
							}
						}
						else
						{
							pbuf.HasTextures.X = 0.0f;
							pbuf.HasTextures.Y = 0.0f;
							pbuf.HasTextures.Z = 0.0f;
						}

						if (prop.ShadowMapPtr != nullptr)
						{
							pbuf.HasTextures.W = 1.0f;
							m_shader->SetTexture("g_shadowTexture", prop.ShadowMapPtr, 3);
						}
						else
						{
							pbuf.HasTextures.W = 0.0f;
						}

						if (prop.SSAOPtr != nullptr)
						{
							m_shader->SetTexture("g_ssaoTexture", prop.SSAOPtr, 4);
						}

						GetGraphics()->SetVertexBuffer(mesh->GetVertexBuffer().get());
						GetGraphics()->SetIndexBuffer(mesh->GetIndexBuffer().get());
						GetGraphics()->SetShader(m_shader.get());

						auto& state = GetGraphics()->GetRenderState()->Push();
						state.DepthTest = true;
						state.DepthWrite = true;
						state.CullingType = CULLING_DOUBLE;
						state.TextureFilterTypes[3] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
						GetGraphics()->GetRenderState()->Update(false);

						GetGraphics()->DrawPolygon(mesh->GetIndexBuffer()->GetCount() / 3);

						GetGraphics()->GetRenderState()->Pop();

						if (fCount + fOffset == bFCount && boneOffsets.size() > bIndex)
						{
							bFCount += boneOffsets[bIndex].FaceOffset;
							bIndex++;
						}

						if (fCount + fOffset == mFCount && materialOffsets.size() > mIndex)
						{
							mFCount += materialOffsets[mIndex].FaceOffset;
							mIndex++;
						}

						fOffset += fCount;
					}
				}
			}
		}
	}

	void RenderedModelObject3D::RenderingShadowMap(RenderingShadowMapProperty& prop)
	{
		auto& vbuf = m_shaderShadow->GetVertexConstantBuffer<VertexConstantBuffer>();
		auto& pbuf = m_shaderShadow->GetPixelConstantBuffer<PixelConstantBuffer>();

		for (auto& g : m_meshGroups_fr)
		{
			auto& matrices = g->m_matrixes_fr;

			for (auto& mesh : g->GetMeshes())
			{
				// 有効チェック
				if (mesh->GetIndexBuffer() == nullptr) continue;

				// 行列計算
				if (matrices.size() > 0)
				{
					// ボーンあり
					for (int32_t i = 0; i < Min(32, matrices.size()); i++)
					{
						vbuf.MMatrices[i].SetIndentity();
						Matrix44::Mul(vbuf.MMatrices[i], GetLocalMatrix_FR(), matrices[i]);
					}
				}
				else
				{
					// ボーンなし
					vbuf.MMatrices[0] = GetLocalMatrix_FR();
					for (int32_t i = 1; i < 32; i++)
					{
						vbuf.MMatrices[i] = vbuf.MMatrices[0];
					}
				}

				{
					vbuf.CMatrix = Matrix44();
					vbuf.PMatrix = prop.LightProjectionMatrix;
					vbuf.LightVPMatrix = prop.LightProjectionMatrix;
				}

				auto& boneOffsets = mesh->GetBoneOffsets();
				auto& materialOffsets = mesh->GetMaterialOffsets();

				{
					// 設定がある場合
					auto mIndex = 0;
					auto bIndex = 0;

					auto fOffset = 0;
					auto fCount = 0;

					auto bFCount = 0;
					if (boneOffsets.size() > 0)
					{
						bFCount = boneOffsets[bIndex].FaceOffset;
					}
					else
					{
						bFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					auto mFCount = 0;
					if (materialOffsets.size() > 0)
					{
						mFCount = materialOffsets[mIndex].FaceOffset;
					}
					else
					{
						mFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					while (fCount < mesh->GetIndexBuffer()->GetCount() / 3)
					{
						fCount = Min(bFCount, mFCount) - fOffset;
						if (fCount == 0) break;

						Mesh_Imp::Material* material = nullptr;
						if (materialOffsets.size() > 0)
						{
							material = mesh->GetMaterial(materialOffsets[mIndex].MaterialIndex);
						}

						if (material != nullptr)
						{
							pbuf.HasTextures.X = material->ColorTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Y = material->NormalTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Z = material->SpecularTexture != nullptr ? 1.0f : 0.0f;

							if (material->ColorTexture != nullptr)
							{
								m_shaderShadow->SetTexture("g_colorTexture", material->ColorTexture, 0);
							}

							if (material->NormalTexture != nullptr)
							{
								m_shaderShadow->SetTexture("g_normalTexture", material->NormalTexture, 1);
							}

							if (material->SpecularTexture != nullptr)
							{
								m_shaderShadow->SetTexture("g_specularTexture", material->SpecularTexture, 2);
							}
						}
						else
						{
							pbuf.HasTextures.X = 0.0f;
							pbuf.HasTextures.Y = 0.0f;
							pbuf.HasTextures.Z = 0.0f;
						}

						GetGraphics()->SetVertexBuffer(mesh->GetVertexBuffer().get());
						GetGraphics()->SetIndexBuffer(mesh->GetIndexBuffer().get());
						GetGraphics()->SetShader(m_shaderShadow.get());

						auto& state = GetGraphics()->GetRenderState()->Push();
						state.DepthTest = true;
						state.DepthWrite = true;
						state.CullingType = CULLING_DOUBLE;
						GetGraphics()->GetRenderState()->Update(false);

						GetGraphics()->DrawPolygon(mesh->GetIndexBuffer()->GetCount() / 3);

						GetGraphics()->GetRenderState()->Pop();

						if (fCount + fOffset == bFCount && boneOffsets.size() > bIndex)
						{
							bFCount += boneOffsets[bIndex].FaceOffset;
							bIndex++;
						}

						if (fCount + fOffset == mFCount && materialOffsets.size() > mIndex)
						{
							mFCount += materialOffsets[mIndex].FaceOffset;
							mIndex++;
						}

						fOffset += fCount;
					}
				}
			}
		}
	}

	void RenderedModelObject3D::RenderingNormalDepth(RenderingProperty& prop)
	{
		auto& vbuf = m_shaderNormalDepth->GetVertexConstantBuffer<VertexConstantBuffer>();
		auto& pbuf = m_shaderNormalDepth->GetPixelConstantBuffer<PixelConstantBuffer>();

		for (auto& g : m_meshGroups_fr)
		{
			auto& matrices = g->m_matrixes_fr;

			for (auto& mesh : g->GetMeshes())
			{
				// 有効チェック
				if (mesh->GetIndexBuffer() == nullptr) continue;

				// 行列計算
				if (matrices.size() > 0)
				{
					// ボーンあり
					for (int32_t i = 0; i < Min(32, matrices.size()); i++)
					{
						vbuf.MMatrices[i].SetIndentity();
						Matrix44::Mul(vbuf.MMatrices[i], GetLocalMatrix_FR(), matrices[i]);
					}
				}
				else
				{
					// ボーンなし
					vbuf.MMatrices[0] = GetLocalMatrix_FR();
					for (int32_t i = 1; i < 32; i++)
					{
						vbuf.MMatrices[i] = vbuf.MMatrices[0];
					}
				}

				{
					vbuf.CMatrix = prop.CameraMatrix;
					vbuf.PMatrix = prop.ProjectionMatrix;
					vbuf.LightVPMatrix = prop.LightProjectionMatrix;
				}

				auto& boneOffsets = mesh->GetBoneOffsets();
				auto& materialOffsets = mesh->GetMaterialOffsets();

				{
					// 設定がある場合
					auto mIndex = 0;
					auto bIndex = 0;

					auto fOffset = 0;
					auto fCount = 0;

					auto bFCount = 0;
					if (boneOffsets.size() > 0)
					{
						bFCount = boneOffsets[bIndex].FaceOffset;
					}
					else
					{
						bFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					auto mFCount = 0;
					if (materialOffsets.size() > 0)
					{
						mFCount = materialOffsets[mIndex].FaceOffset;
					}
					else
					{
						mFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					while (fCount < mesh->GetIndexBuffer()->GetCount() / 3)
					{
						fCount = Min(bFCount, mFCount) - fOffset;
						if (fCount == 0) break;

						Mesh_Imp::Material* material = nullptr;
						if (materialOffsets.size() > 0)
						{
							material = mesh->GetMaterial(materialOffsets[mIndex].MaterialIndex);
						}

						if (material != nullptr)
						{
							pbuf.HasTextures.X = material->ColorTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Y = material->NormalTexture != nullptr ? 1.0f : 0.0f;
							pbuf.HasTextures.Z = material->SpecularTexture != nullptr ? 1.0f : 0.0f;

							if (material->ColorTexture != nullptr)
							{
								m_shaderNormalDepth->SetTexture("g_colorTexture", material->ColorTexture, 0);
							}

							if (material->NormalTexture != nullptr)
							{
								m_shaderNormalDepth->SetTexture("g_normalTexture", material->NormalTexture, 1);
							}

							if (material->SpecularTexture != nullptr)
							{
								m_shaderNormalDepth->SetTexture("g_specularTexture", material->SpecularTexture, 2);
							}
						}
						else
						{
							pbuf.HasTextures.X = 0.0f;
							pbuf.HasTextures.Y = 0.0f;
							pbuf.HasTextures.Z = 0.0f;
						}

						GetGraphics()->SetVertexBuffer(mesh->GetVertexBuffer().get());
						GetGraphics()->SetIndexBuffer(mesh->GetIndexBuffer().get());
						GetGraphics()->SetShader(m_shaderNormalDepth.get());

						auto& state = GetGraphics()->GetRenderState()->Push();
						state.DepthTest = true;
						state.DepthWrite = true;
						state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_OPACITY;
						state.CullingType = CULLING_DOUBLE;
						GetGraphics()->GetRenderState()->Update(false);

						GetGraphics()->DrawPolygon(mesh->GetIndexBuffer()->GetCount() / 3);

						GetGraphics()->GetRenderState()->Pop();

						if (fCount + fOffset == bFCount && boneOffsets.size() > bIndex)
						{
							bFCount += boneOffsets[bIndex].FaceOffset;
							bIndex++;
						}

						if (fCount + fOffset == mFCount && materialOffsets.size() > mIndex)
						{
							mFCount += materialOffsets[mIndex].FaceOffset;
							mIndex++;
						}

						fOffset += fCount;
					}
				}
			}
		}
	}
}