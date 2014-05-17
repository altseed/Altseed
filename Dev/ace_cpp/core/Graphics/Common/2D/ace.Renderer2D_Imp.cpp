
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Renderer2D_Imp.h"

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

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos.x = Input.Pos.x / Size.x * 2.0 - 1.0;
	Output.Pos.y = -(Input.Pos.y / Size.y * 2.0 - 1.0);
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

static const char* dx_nt_vs = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float4 Color	: COLOR0;
};

float4 Size			: register( c0 );

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos.x = Input.Pos.x / Size.x * 2.0 - 1.0;
	Output.Pos.y = -(Input.Pos.y / Size.y * 2.0 - 1.0);
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;
	Output.Color = Input.Color;
	return Output;
}

)";

static const char* dx_nt_ps = R"(

struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float4 Color	: COLOR0;
};


float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color;
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

void main()
{
	gl_Position.x = Pos.x / Size.x * 2.0 - 1.0;
	gl_Position.y = -(Pos.y / Size.y * 2.0 - 1.0);
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


static const char* gl_nt_vs = R"(

attribute vec3 Pos;
attribute vec2 UV;
attribute vec4 Color;

varying vec4 vaColor;

uniform vec4 Size;

void main()
{
	gl_Position.x = Pos.x / Size.x * 2.0 - 1.0;
	gl_Position.y = -(Pos.y / Size.y * 2.0 - 1.0);
	gl_Position.z = 0.5;
	gl_Position.w = 1.0;
	vaColor = Color;
}

)";

static const char* gl_nt_ps = R"(

varying vec4 vaColor;

void main() 
{
	gl_FragColor = vaColor;
}

)";

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D::Renderer2D()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D::~Renderer2D()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D_Imp::Renderer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: m_graphics(nullptr)
		, m_log(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		m_log = log;

		SafeAddRef(graphics);

		m_windowSize = windowSize;
		

		m_vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(SpriteVertex), SpriteCount * 4, true);
		m_indexBuffer = m_graphics->CreateIndexBuffer_Imp(SpriteCount*6, false, false);
		
		{
			m_indexBuffer->Lock();
			auto ib = m_indexBuffer->GetBuffer<uint16_t>(SpriteCount*6);
			
			for (int32_t i = 0; i < SpriteCount; i++)
			{
				ib[i * 6 + 0] = 0 + i * 4;
				ib[i * 6 + 1] = 1 + i * 4;
				ib[i * 6 + 2] = 2 + i * 4;
				ib[i * 6 + 3] = 0 + i * 4;
				ib[i * 6 + 4] = 2 + i * 4;
				ib[i * 6 + 5] = 3 + i * 4;
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

		if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
		{
			m_shader_nt = m_graphics->CreateShader_Imp(
				gl_nt_vs,
				"vs",
				gl_nt_ps,
				"ps",
				vl,
				macro);
		}
		else
		{
			m_shader_nt = m_graphics->CreateShader_Imp(
				dx_nt_vs,
				"vs",
				dx_nt_ps,
				"ps",
				vl,
				macro);
		}

		std::vector<ace::ConstantBufferInformation> constantBuffers;
		constantBuffers.resize(1);
		constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
		constantBuffers[0].Name = std::string("Size");
		constantBuffers[0].Offset = 0;

		m_shader->CreateVertexConstantBuffer<SpriteConstantBuffer>(constantBuffers);
		m_shader_nt->CreateVertexConstantBuffer<SpriteConstantBuffer>(constantBuffers);

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D_Imp::~Renderer2D_Imp()
	{
		ClearCache();

		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_shader_nt.reset();
		m_shader.reset();

		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::DrawCache()
	{
		StartDrawing();

		for (auto& c : m_events)
		{
			for (auto& e : c.second)
			{
				Draw(e);
			}
		}

		EndDrawing();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::ClearCache()
	{
		for (auto& c : m_events)
		{
			for (auto& e : c.second)
			{
				if (e.Type == Event::eEventType::Sprite)
				{
					SafeRelease(e.Data.Sprite.Texture);
				}
			}

			c.second.clear();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::AddSprite(Vector2DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, eAlphaBlend alphaBlend, int32_t priority)
	{
		Event e;
		e.Type = Event::eEventType::Sprite;

		memcpy(e.Data.Sprite.Positions, positions, sizeof(ace::Vector2DF) * 4);
		memcpy(e.Data.Sprite.Colors, colors, sizeof(ace::Color) * 4);
		memcpy(e.Data.Sprite.UV, uv, sizeof(ace::Vector2DF) * 4);
		e.Data.Sprite.AlphaBlend = alphaBlend;
		e.Data.Sprite.Texture = texture;
		SafeAddRef(e.Data.Sprite.Texture);

		AddEvent(priority, e);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::AddEvent(int32_t priority, Event& e)
	{
		if (m_events.count(priority) == 0)
		{
			m_events[priority] = std::vector<Event>();
		}
		
		m_events[priority].push_back(e);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::StartDrawing()
	{
		
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::Draw(Event& e)
	{
		auto resetState = [this,e]() -> void
		{
			m_state.Texture = e.Data.Sprite.Texture;
			m_state.AlphaBlend = e.Data.Sprite.AlphaBlend;
		};

		if (e.Type == Event::eEventType::Sprite)
		{
			if (m_drawingSprites.size() == 0)
			{
				// 初期値設定
				resetState();
			}
			else
			{
				// 同時描画不可のケースかどうか?
				// もしくはバッファが溢れないかどうか?
				if (m_state.Texture != e.Data.Sprite.Texture ||
					m_state.AlphaBlend != e.Data.Sprite.AlphaBlend||
					m_drawingSprites.size() >= SpriteCount)
				{
					DrawSprite();
					resetState();
				}
			}

			// 書き込み
			m_drawingSprites.push_back(&e);
		}
		else
		{
			if (m_drawingSprites.size() != 0) DrawSprite();
		}

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::EndDrawing()
	{
		DrawSprite();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::DrawSprite()
	{
		if (m_drawingSprites.size() == 0) return;

		// 頂点情報をビデオメモリに転送
		if (!m_vertexBuffer->RingBufferLock(m_drawingSprites.size() * 4))
		{
			assert(0);
		}

		//m_vertexBuffer->Lock();
		auto buf = m_vertexBuffer->GetBuffer < SpriteVertex>(m_drawingSprites.size() * 4);

		int32_t ind = 0;
		for (auto& e : m_drawingSprites)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				buf[ind + i].Position.X = e->Data.Sprite.Positions[i].X;
				buf[ind + i].Position.Y = e->Data.Sprite.Positions[i].Y;
				buf[ind + i].Position.Z = 0.5f;
				buf[ind + i].UV.X = e->Data.Sprite.UV[i].X;
				buf[ind + i].UV.Y = e->Data.Sprite.UV[i].Y;
				buf[ind + i].Color_.R = e->Data.Sprite.Colors[i].R;
				buf[ind + i].Color_.G = e->Data.Sprite.Colors[i].G;
				buf[ind + i].Color_.B = e->Data.Sprite.Colors[i].B;
				buf[ind + i].Color_.A = e->Data.Sprite.Colors[i].A;
			}
			ind += 4;
		}

		m_vertexBuffer->Unlock();

		// テクスチャの有無でシェーダーを選択
		std::shared_ptr<NativeShader_Imp> shader;
		if (m_state.Texture != nullptr)
		{
			shader = m_shader;
		}
		else
		{
			shader = m_shader_nt;
		}

		// 定数バッファを設定
		Vector4DF windowSize = Vector4DF(m_windowSize.X, m_windowSize.Y, 0, 0);
		shader->SetConstantBuffer("Size", &windowSize, sizeof(Vector4DF));


		// 描画
		if (m_state.Texture != nullptr)
		{
			shader->SetTexture("g_texture", m_state.Texture, 0);
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

		m_graphics->DrawPolygon(m_drawingSprites.size() * 2);

		m_graphics->GetRenderState()->Pop();

		m_drawingSprites.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
