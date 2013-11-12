
#include "ace.RenderedMeshObject3D.h"
#include "ace.Mesh.h"

#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"


namespace ace
{
	static const char* dx_vs = R"(

float4x4	matMCP						: register( c0 );
float3		directionalLightDirection	: register( c4 );
float3		directionalLightColor		: register( c5 );

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

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos = mul( float4( Input.Position.x, Input.Position.y, Input.Position.z, 1.0 ), matMCP );
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
attribute vec4 BoneIndexesOriginal;

uniform mat4		matMCP;
uniform vec3		directionalLightDirection;
uniform vec3		directionalLightColor;

varying vec4 vaTexCoord;
varying vec4 vaColor;

void main()
{
	gl_Position = vec4(Position.x,Position.y,Position.z,1.0) * matMCP;
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

	RenderedMeshObject3D::RenderedMeshObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_mesh(nullptr)
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
			constantBuffers[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers[0].Name = std::string("matMCP");
			constantBuffers[0].Offset = 0;

			constantBuffers[1].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers[1].Name = std::string("directionalLightDirection");
			constantBuffers[1].Offset = sizeof(Matrix44);

			constantBuffers[2].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers[2].Name = std::string("directionalLightColor");
			constantBuffers[2].Offset = sizeof(Matrix44) +sizeof(float) * 4;

			m_shader->CreateVertexConstantBuffer<VertexConstantBuffer>(constantBuffers);
		}

		RenderedMeshObject3D::~RenderedMeshObject3D()
		{
			SafeRelease(m_mesh);
		}

		void RenderedMeshObject3D::SetMesh(Mesh* mesh)
		{
			SafeSubstitute(m_mesh, mesh);
		}


		void RenderedMeshObject3D::Flip()
		{
			RenderedObject3D::Flip();
		}

		void RenderedMeshObject3D::Rendering(RenderingProperty& prop)
		{
			auto& vbuf = m_shader->GetVertexConstantBuffer<VertexConstantBuffer>();
			Matrix44::Mul(vbuf.MCPMatrix, GetLocalMatrix_FR(), prop.CameraProjectionMatrix);

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