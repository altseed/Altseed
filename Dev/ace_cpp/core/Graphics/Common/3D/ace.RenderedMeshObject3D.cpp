
#include "ace.RenderedMeshObject3D.h"
#include "ace.Mesh_Imp.h"
#include "ace.Armature_Imp.h"

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

float4x4	matMCP[32]					: register( c0 );
float3		directionalLightDirection	: register( c128 );
float3		directionalLightColor		: register( c129 );

struct VS_Input
{
	float3 Position		: Position0;
	float3 Normal		: Normal0;
	float3 Binormal		: Binormal0;
	float2 UV			: UV0;
	float2 UVSub		: UVSub0;
	float4 Color		: Color0;
	float4 BoneWeights	: BoneWeights0;
	float4 BoneIndexes	: BoneIndexes0;
	float4 BoneIndexesOriginal	: BoneIndexesOriginal0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
};

float4x4 calcMatrix(float4 weights, float4 indexes)
{
	return matMCP[indexes.x] * weights.x +
	matMCP[indexes.y] * weights.y +
	matMCP[indexes.z] * weights.z +
	matMCP[indexes.w] * weights.w;
}

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	//Output.Pos = mul( float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ), matMCP[0] );
	Output.Pos = mul( float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ), calcMatrix(Input.BoneWeights,Input.BoneIndexes) );
	
	Output.UV = Input.UV;
	Output.Color.xyz = directionalLightColor * max( dot(directionalLightDirection,Input.Normal), 0.0 );
	Output.Color.w = 1.0;
	return Output;
}

)";

	static const char* dx_ps = R"(

struct PS_Input
{
	float4 Position	: SV_POSITION;
	float4 Color	: Color0;
	float2 UV		: TEXCOORD0;
};


float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
	if(Output.a == 0.0f) discard;
	return Output;
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

uniform mat4		matMCP[32];
uniform vec3		directionalLightDirection;
uniform vec3		directionalLightColor;

varying vec4 vaTexCoord;
varying vec4 vaColor;

mat4 calcMatrix(vec4 weights, vec4 indexes)
{
	return matMCP[int(indexes.x)] * weights.x +
	matMCP[int(indexes.y)] * weights.y +
	matMCP[int(indexes.z)] * weights.z +
	matMCP[int(indexes.w)] * weights.w;
}

void main()
{
//	gl_Position = vec4(Position.x,Position.y,Position.z,1.0) * matMCP[0];
	gl_Position = vec4(Position.x,Position.y,Position.z,1.0) * calcMatrix(BoneWeights,BoneIndexes);
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
	vaColor.xyz = directionalLightColor * max( dot(directionalLightDirection,Normal), 0.0 );
	vaColor.w = 1.0;
}

)";

static const char* gl_ps = R"(

varying vec4 vaTexCoord;
varying vec4 vaColor;

void main() 
{
	gl_FragColor = vaColor;
}

)";


	RenderedMeshObject3D::BoneProperty::BoneProperty()
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


	Matrix44 RenderedMeshObject3D::BoneProperty::CalcMatrix(eBoneRotationType rotationType)
	{

		if (rotationType == BONE_ROTATION_TYPE_XYZ ||
			rotationType == BONE_ROTATION_TYPE_XZY)
		{
			Matrix44 mat, matS, matRx, matRy, matRz, matT;
			matS.Scaling(Scale[0], Scale[1], Scale[2]);
			matT.Translation(Position[0], Position[1], Position[2]);
			matRx.RotationX(Rotation[0]);
			matRy.RotationY(Rotation[2]);
			matRz.RotationZ(-Rotation[1]);

			if (rotationType == BONE_ROTATION_TYPE_XZY)
			{
				mat = Matrix44::Mul(mat, matS, matRx);
				mat = Matrix44::Mul(mat, mat, matRz);
				mat = Matrix44::Mul(mat, mat, matRy);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			if (rotationType == BONE_ROTATION_TYPE_XYZ)
			{
				mat = Matrix44::Mul(mat, matS, matRx);
				mat = Matrix44::Mul(mat, mat, matRy);
				mat = Matrix44::Mul(mat, mat, matRz);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			if (rotationType == BONE_ROTATION_TYPE_ZXY)
			{
				mat = Matrix44::Mul(mat, matS, matRz);
				mat = Matrix44::Mul(mat, mat, matRx);
				mat = Matrix44::Mul(mat, mat, matRy);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			if (rotationType == BONE_ROTATION_TYPE_ZYX)
			{
				mat = Matrix44::Mul(mat, matS, matRz);
				mat = Matrix44::Mul(mat, mat, matRy);
				mat = Matrix44::Mul(mat, mat, matRz);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			if (rotationType == BONE_ROTATION_TYPE_YXZ)
			{
				mat = Matrix44::Mul(mat, matS, matRy);
				mat = Matrix44::Mul(mat, mat, matRx);
				mat = Matrix44::Mul(mat, mat, matRz);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			if (rotationType == BONE_ROTATION_TYPE_YZX)
			{
				mat = Matrix44::Mul(mat, matS, matRy);
				mat = Matrix44::Mul(mat, mat, matRz);
				mat = Matrix44::Mul(mat, mat, matRx);
				mat = Matrix44::Mul(mat, mat, matT);
			}

			return mat;
		}
		else if (rotationType == BONE_ROTATION_TYPE_QUATERNION)
		{
			Matrix44 mat, matS, matR, matT;
			matS.Scaling(Scale[0], Scale[1], Scale[2]);
			matT.Translation(Position[0], Position[1], Position[2]);
			matR.Quaternion(Rotation[0], Rotation[1], Rotation[2], Rotation[3]);

			mat = Matrix44::Mul(mat, matS, matR);
			mat = Matrix44::Mul(mat, mat, matT);

			return mat;
		}
		else if (rotationType == BONE_ROTATION_TYPE_AXIS)
		{
			Matrix44 mat, matS, matR, matT;
			matS.Scaling(Scale[0], Scale[1], Scale[2]);
			matT.Translation(Position[0], Position[1], Position[2]);
			matR.RotationAxis(Vector3DF(Rotation[0], Rotation[2], -Rotation[1]), Rotation[3]);

			mat = Matrix44::Mul(mat, matS, matR);
			mat = Matrix44::Mul(mat, mat, matT);

			return mat;
		}

		return Matrix44();
	}

	RenderedMeshObject3D::RenderedMeshObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_mesh(nullptr)
		, m_armature(nullptr)
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
			vl.push_back(ace::VertexLayout("BoneIndexes", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
			vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

			std::shared_ptr<ace::NativeShader_Imp> shader;
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

			std::vector<ace::ConstantBufferInformation> constantBuffers;
			constantBuffers.resize(3);
			constantBuffers[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
			constantBuffers[0].Name = std::string("matMCP");
			constantBuffers[0].Offset = 0;
			constantBuffers[0].Count = 32;

			constantBuffers[1].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers[1].Name = std::string("directionalLightDirection");
			constantBuffers[1].Offset = sizeof(Matrix44) * 32;

			constantBuffers[2].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers[2].Name = std::string("directionalLightColor");
			constantBuffers[2].Offset = sizeof(Matrix44) * 32 + sizeof(float) * 4;

			m_shader->CreateVertexConstantBuffer<VertexConstantBuffer>(constantBuffers);
		}

		RenderedMeshObject3D::~RenderedMeshObject3D()
		{
			SafeRelease(m_mesh);
			SafeRelease(m_armature);

			for (auto& a : m_animationClips)
			{
				a.second->Release();
			}
			m_animationClips.clear();
		}

		void RenderedMeshObject3D::SetMesh(Mesh* mesh)
		{
			auto m = (Mesh_Imp*) mesh;
			SafeSubstitute(m_mesh, m);
		}

		void RenderedMeshObject3D::SetArmature(Armature* armature)
		{
			auto a = (Armature_Imp*) armature;
			SafeSubstitute(m_armature, a);

			m_matrixes.resize(m_armature->GetBones().size());
			m_boneProps.resize(m_armature->GetBones().size());

			for (int32_t i = 0; i < m_boneProps.size(); i++)
			{
				m_boneProps[i] = BoneProperty();
			}
		}

		void RenderedMeshObject3D::AddAnimationClip(const achar* name, AnimationClip* animationClip)
		{
			if (animationClip == nullptr) return;

			if (m_animationClips.find(name) == m_animationClips.end())
			{
				SafeAddRef(animationClip);
				m_animationClips[name] = animationClip;
			}
		}

		void RenderedMeshObject3D::PlayAnimation(const achar* name)
		{
			auto it = m_animationClips.find(name);
			if (it == m_animationClips.end()) return;

			m_animationPlaying = (*it).second;
			m_animationTime = 0;
		}

		void RenderedMeshObject3D::Flip()
		{
			RenderedObject3D::Flip();

			if (m_armature == nullptr) return;

			auto localMatrix = CalcLocalMatrix();

			// アニメーションの適用
			if (m_animationPlaying != nullptr)
			{
				auto source = (AnimationSource_Imp*)m_animationPlaying->GetSource().get();
				auto& animations = source->GetAnimations();
				
				for (auto& a : animations)
				{
					auto a_ = (KeyframeAnimation_Imp*) a;

					auto type = a_->GetTargetType();
					auto axis = a_->GetTargetAxis();
					auto bi = m_armature->GetBoneIndex(a_->GetTargetName());
					
					if (type == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_NONE) continue;
					if (axis == eAnimationCurveTargetAxis::ANIMATION_CURVE_TARGET_AXIS_NONE) continue;
					if (bi < 0) continue;
					
					auto value = a_->GetValue(m_animationTime);

					if (type == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_POSITON)
					{
						m_boneProps[bi].Position[axis] = value;
					}
					else if (type == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_ROTATION)
					{
						m_boneProps[bi].Rotation[axis] = value;
					}
					else if (type == eAnimationCurveTargetType::ANIMATION_CURVE_TARGET_TYPE_SCALE)
					{
						m_boneProps[bi].Scale[axis] = value;
					}
				}

				m_animationTime++;
			}

			// 計算
			for (auto i = 0; i < m_armature->GetBones().size(); i++)
			{
				auto& b = m_armature->GetBones()[i];

				// ローカル行列の計算
				m_matrixes[i] = m_boneProps[i].CalcMatrix(b.RotationType);

				Matrix44::Mul(m_matrixes[i], m_matrixes[i], b.LocalMat);

				if (b.ParentBoneIndex >= 0)
				{
					m_matrixes[i] = Matrix44();
					Matrix44::Mul(m_matrixes[i], m_matrixes[i], m_matrixes[b.ParentBoneIndex]);
				}
				else
				{
					Matrix44::Mul(m_matrixes[i], m_matrixes[i], localMatrix);
				}
			}

			for (auto i = 0; i < m_armature->GetBones().size(); i++)
			{
				auto& b = m_armature->GetBones()[i];
				Matrix44::Mul(m_matrixes[i], m_matrixes[i], b.GlobalMatInv);
			}

			// コピー
			if (m_matrixes_fr.size() != m_matrixes.size())
			{
				m_matrixes_fr.resize(m_matrixes.size());
			}

			std::copy(m_matrixes.begin(), m_matrixes.end(), std::back_inserter(m_matrixes_fr));
		}

		void RenderedMeshObject3D::Rendering(RenderingProperty& prop)
		{
			auto& vbuf = m_shader->GetVertexConstantBuffer<VertexConstantBuffer>();

			// 行列計算
			for (int32_t i = 0; i < Min(32, m_matrixes_fr.size()); i++)
			{
				vbuf.MCPMatrices[i].Indentity();
				Matrix44::Mul(vbuf.MCPMatrices[i], GetLocalMatrix_FR(), prop.CameraProjectionMatrix);
				Matrix44::Mul(vbuf.MCPMatrices[i], m_matrixes_fr[i], vbuf.MCPMatrices[i]);
			}

			{
				vbuf.DirectionalLightDirection = prop.DirectionalLightDirection;
				vbuf.DirectionalLightColor.X = prop.DirectionalLightColor.R / 255.0f;
				vbuf.DirectionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f;
				vbuf.DirectionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f;
			}

			GetGraphics()->SetVertexBuffer(m_mesh->GetVertexBuffer().get());
			GetGraphics()->SetIndexBuffer(m_mesh->GetIndexBuffer().get());
			GetGraphics()->SetShader(m_shader.get());

			auto& state = GetGraphics()->GetRenderState()->Push();
			state.DepthTest = true;
			state.DepthWrite = true;
			state.CullingType = CULLING_DOUBLE;
			GetGraphics()->GetRenderState()->Update(false);

			GetGraphics()->DrawPolygon(m_mesh->GetIndexBuffer()->GetCount() / 3);

			GetGraphics()->GetRenderState()->Pop();
		}
}