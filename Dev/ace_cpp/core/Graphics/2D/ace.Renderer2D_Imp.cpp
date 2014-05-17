
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Renderer2D_Imp.h"

#include "../../Log/ace.Log.h"
#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"
#include "../Resource/ace.ShaderCache.h"

#include <Utility/ace.TypeErasureCopy.h>

#include "../Shader/DX/2D/Renderer2D_PS.h"
#include "../Shader/DX/2D/Renderer2D_VS.h"
#include "../Shader/GL/2D/Renderer2D_PS.h"
#include "../Shader/GL/2D/Renderer2D_VS.h"

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

		std::vector<ace::Macro> macro_tex;
		macro_tex.push_back(Macro("HAS_TEXTURE", "1"));

		std::vector<ace::Macro> macro;
		
		if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.Renderer2D.Texture").c_str(),
				renderer2d_vs_gl,
				renderer2d_ps_gl,
				vl,
				macro_tex);
		}
		else
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.Renderer2D.Texture").c_str(),
				renderer2d_vs_dx,
				renderer2d_ps_dx,
				vl,
				macro_tex);
		}

		if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
		{
			m_shader_nt = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.Renderer2D").c_str(),
				renderer2d_vs_gl,
				renderer2d_ps_gl,
				vl,
				macro);
		}
		else
		{
			m_shader_nt = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.Renderer2D").c_str(),
				renderer2d_vs_dx,
				renderer2d_ps_dx,
				vl,
				macro);
		}
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
		shader->SetVector4DF("Size", windowSize);

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
