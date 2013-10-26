
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.LayerRenderer.h"

#include "../../../Log/ace.Log.h"
#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"

#include <Utility/ace.TypeErasureCopy.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static const char* dx_vs = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};

float4 Size			: register( c0 );
float4 LayerPosU	: register( c1 );
float4 LayerPosL	: register( c2 );

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	float2 ul = LayerPosU.xy / Size.xy;
	float2 ur = LayerPosU.zw / Size.xy;
	float2 lr = LayerPosL.xy / Size.xy;
	float2 ll = LayerPosL.zw / Size.xy;

	float2 pos = float2( (Input.Pos.x + 1.0) / 2.0, 1.0 - (Input.Pos.y + 1.0) / 2.0 );
  
	float2 u = (ur - ul) * pos.x + ul;
	float2 l = (lr - ll) * pos.x + ll;
	float2 p = (l - u) * pos.y + u;

	Output.Pos.x = p.x * 2.0 - 1.0;
	Output.Pos.y = -(p.y * 2.0 - 1.0);
 
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

	Output.UV = Input.UV;
	Output.Color = Input.Color;
	return Output;
}

)";

static const char* dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );


struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};


float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = g_texture.Sample(g_sampler, Input.UV) * Input.Color;
	if(Output.a == 0.0f) discard;
	return Output;
}

)";

static const char* gl_vs = R"(

attribute vec3 Pos;
attribute vec2 UV;
attribute vec4 Color;

varying vec4 vaTexCoord;
varying vec4 vaColor;

uniform vec4 Size;
uniform vec4 LayerPosU;
uniform vec4 LayerPosL;

void main()
{
	vec2 ul = LayerPosU.xy / Size.xy;
	vec2 ur = LayerPosU.zw / Size.xy;
	vec2 lr = LayerPosL.xy / Size.xy;
	vec2 ll = LayerPosL.zw / Size.xy;

	vec2 pos = vec2( (Pos.x + 1.0) / 2.0, 1.0 - (Pos.y + 1.0) / 2.0 );
  
	vec2 u = (ur - ul) * pos.x + ul;
	vec2 l = (lr - ll) * pos.x + ll;
	vec2 p = (l - u) * pos.y + u;

	gl_Position.x = p.x * 2.0 - 1.0;
	gl_Position.y = -(p.y * 2.0 - 1.0);

	gl_Position.z = 0.5;
	gl_Position.w = 1.0;

	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
	vaColor = Color;
}

)";

static const char* gl_ps = R"(

varying vec4 vaTexCoord;
varying vec4 vaColor;

uniform sampler2D g_texture;

void main() 
{
	// varying(in) は変更不可(Radeon)

	// gl only
	vec4 vaTexCoord_ = vaTexCoord;
	vaTexCoord_.y = 1.0 - vaTexCoord_.y;

	gl_FragColor = texture2D(g_texture, vaTexCoord_.xy) * vaColor;
}

)";


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	LayerRenderer::LayerRenderer(Graphics* graphics)
		: m_graphics(nullptr)
		, m_texture(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;

		SafeAddRef(graphics);

		m_vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(TriangleVertex), TriangleCount * 3, true);
		m_indexBuffer = m_graphics->CreateIndexBuffer_Imp(TriangleCount * 3, false, false);

		{
			m_indexBuffer->Lock();
			auto ib = m_indexBuffer->GetBuffer<uint16_t>(TriangleCount * 3);

			for (int32_t i = 0; i < TriangleCount; i++)
			{
				ib[i * 3 + 0] = 0 + i * 3;
				ib[i * 3 + 1] = 1 + i * 3;
				ib[i * 3 + 2] = 2 + i * 3;
			}

			m_indexBuffer->Unlock();
	}

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

		std::vector<ace::Macro> macro;
		if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
		{
			m_shader = m_graphics->CreateShader_Imp(
				gl_vs,
				"vs",
				gl_ps,
				"ps",
				vl,
				macro);
		}
		else
		{
			m_shader = m_graphics->CreateShader_Imp(
				dx_vs,
				"vs",
				dx_ps,
				"ps",
				vl,
				macro);
		}


		std::vector<ace::ConstantBufferInformation> constantBuffers;
		constantBuffers.resize(3);
		constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
		constantBuffers[0].Name = std::string("Size");
		constantBuffers[0].Offset = 0;

		constantBuffers[1].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
		constantBuffers[1].Name = std::string("LayerPosU");
		constantBuffers[1].Offset = sizeof(float) * 4;

		constantBuffers[2].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
		constantBuffers[2].Name = std::string("LayerPosL");
		constantBuffers[2].Offset = sizeof(float) * 8;

		m_shader->CreateVertexConstantBuffer<TriangleConstantBuffer>(constantBuffers);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	LayerRenderer::~LayerRenderer()
	{
		ClearCache();

		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_shader.reset();

		SafeRelease(m_texture);

		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::DrawCache()
	{
		if (m_texture == nullptr) return;

		int32_t offset = 0;
		while (offset < m_vertecies.size())
		{
			int32_t vCount = (m_vertecies.size() - offset);

			// 頂点情報をビデオメモリに転送
			if (!m_vertexBuffer->RingBufferLock(vCount))
			{
				assert(0);
			}

			auto buf = m_vertexBuffer->GetBuffer <TriangleVertex>(vCount);

			for (int32_t i = 0; i < vCount; i++)
			{
				buf[i] = m_vertecies[offset + i];
			}
			m_vertexBuffer->Unlock();

			// テクスチャの有無でシェーダーを選択
			std::shared_ptr<NativeShader_Imp> shader = m_shader;

			// 定数バッファを設定
			auto& cbuf = shader->GetVertexConstantBuffer<TriangleConstantBuffer>();
			cbuf.Size.X = m_windowSize.X;
			cbuf.Size.Y = m_windowSize.Y;
			cbuf.UL = m_layerPosition[0];
			cbuf.UR = m_layerPosition[1];
			cbuf.LR = m_layerPosition[2];
			cbuf.LL = m_layerPosition[3];

			// 描画
			if (m_texture != nullptr)
			{
				shader->SetTexture("g_texture", m_texture, 0);
			}
			m_graphics->SetVertexBuffer(m_vertexBuffer.get());
			m_graphics->SetIndexBuffer(m_indexBuffer.get());
			m_graphics->SetShader(shader.get());

			auto& state = m_graphics->GetRenderState()->Push();
			state.DepthTest = false;
			state.DepthWrite = false;
			state.CullingType = ace::eCullingType::CULLING_DOUBLE;
			state.TextureWrapTypes[0] = ace::eTextureWrapType::TEXTURE_WRAP_CLAMP;
			m_graphics->GetRenderState()->Update(false);

			m_graphics->DrawPolygon(vCount / 3);

			m_graphics->GetRenderState()->Pop();

			offset += (m_vertecies.size() * 3);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::ClearCache()
	{
		m_vertecies.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::AddTriangle(Vector2DF positions[3], Color colors[3], Vector2DF uv[3])
	{
		TriangleVertex v;
		for (int32_t i = 0; i < 3; i++)
		{
			v.Position.X = positions[i].X;
			v.Position.Y = positions[i].Y;
			v.Position.Z = 0.5f;

			v.UV = uv[i];
			v.Col = colors[i];
			m_vertecies.push_back(v);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::SetWindowSize(Vector2DI windowSize)
	{
		m_windowSize = windowSize;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::SetLayerPosition(Vector2DF layerPosition[4])
	{
		memcpy(m_layerPosition, layerPosition, sizeof(Vector2DF) * 4);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void LayerRenderer::SetTexture(Texture2D* texture)
	{
		SafeSubstitute(m_texture, texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
