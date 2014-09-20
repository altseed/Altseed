
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.SpriteRenderer3D.h"

#include "../../../Log/ace.Log.h"
#include "../../ace.Graphics_Imp.h"
#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.NativeShader_Imp.h"
#include "../../Resource/ace.ShaderCache.h"

#include <Utility/ace.TypeErasureCopy.h>

#include "../../Shader/DX/3D/SpriteRenderer3D_PS.h"
#include "../../Shader/DX/3D/SpriteRenderer3D_VS.h"
#include "../../Shader/GL/3D/SpriteRenderer3D_PS.h"
#include "../../Shader/GL/3D/SpriteRenderer3D_VS.h"

#if _WIN32
#include "../../Platform/DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../../Platform/GL/ace.Graphics_Imp_GL.h"

namespace ace {

	SpriteRenderer3D::SpriteRenderer3D(Graphics* graphics, Log* log)
		: m_graphics(nullptr)
		, m_log(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		m_log = log;

		SafeAddRef(graphics);

		m_vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(SpriteVertex), SpriteCount * 4, true);
		m_indexBuffer = m_graphics->CreateIndexBuffer_Imp(SpriteCount * 6, false, false);

		{
			m_indexBuffer->Lock();
			auto ib = m_indexBuffer->GetBuffer<uint16_t>(SpriteCount * 6);

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

		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.3D.SpriteRenderer3D.Texture").c_str(),
				sprite_renderer3d_vs_gl,
				sprite_renderer3d_ps_gl,
				vl,
				macro_tex);
		}
		else
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.3D.SpriteRenderer3D.Texture").c_str(),
				sprite_renderer3d_vs_dx,
				sprite_renderer3d_ps_dx,
				vl,
				macro_tex);
		}

		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			m_shader_nt = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.3D.SpriteRenderer3D").c_str(),
				sprite_renderer3d_vs_gl,
				sprite_renderer3d_ps_gl,
				vl,
				macro);
		}
		else
		{
			m_shader_nt = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.3D.SpriteRenderer3D").c_str(),
				sprite_renderer3d_vs_dx,
				sprite_renderer3d_ps_dx,
				vl,
				macro);
		}
	}

	SpriteRenderer3D::~SpriteRenderer3D()
	{
		ClearCache();

		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_shader_nt.reset();
		m_shader.reset();

		SafeRelease(m_graphics);
	}

	void SpriteRenderer3D::DrawCache()
	{
		StartDrawing();

		for (auto& s : sprites)
		{
			Draw(s);
		}

		EndDrawing();
	}

	void SpriteRenderer3D::ClearCache()
	{
		for (auto& s : sprites)
		{
			SafeRelease(s.TexturePtr);
		}

		sprites.clear();
	}

	void SpriteRenderer3D::AddSprite(Vector3DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, AlphaBlend alphaBlend)
	{
		Sprite s;
		s.TexturePtr = texture;
		SafeAddRef(s.TexturePtr);
		s.AlphaBlendState = alphaBlend;
		memcpy(s.Positions, positions, sizeof(ace::Vector2DF) * 4);
		memcpy(s.Colors, colors, sizeof(ace::Color) * 4);
		memcpy(s.UV, uv, sizeof(ace::Vector2DF) * 4);

		sprites.push_back(s);
	}

	void SpriteRenderer3D::StartDrawing()
	{
		
	}

	void SpriteRenderer3D::Draw(Sprite& s)
	{
		auto resetState = [this,&s]() -> void
		{
			m_state.TexturePtr = s.TexturePtr;
			m_state.AlphaBlendState = s.AlphaBlendState;
		};

		if (drawingSprites.size() == 0)
		{
			// 初期値設定
			resetState();
		}
		else
		{
			// 同時描画不可のケースかどうか?
			// もしくはバッファが溢れないかどうか?
			if (m_state.TexturePtr != s.TexturePtr ||
				m_state.AlphaBlendState != s.AlphaBlendState ||
				drawingSprites.size() >= SpriteCount)
			{
				DrawSprite();
				resetState();
			}
		}

		// 書き込み
		drawingSprites.push_back(&s);
	}

	void SpriteRenderer3D::EndDrawing()
	{
		DrawSprite();
	}

	void SpriteRenderer3D::DrawSprite()
	{
		if (drawingSprites.size() == 0) return;

		// 頂点情報をビデオメモリに転送
		if (!m_vertexBuffer->RingBufferLock(drawingSprites.size() * 4))
		{
			assert(0);
		}

		//m_vertexBuffer->Lock();
		auto buf = m_vertexBuffer->GetBuffer < SpriteVertex>(drawingSprites.size() * 4);

		int32_t ind = 0;
		for (auto& s : drawingSprites)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				buf[ind + i].Position.X = s->Positions[i].X;
				buf[ind + i].Position.Y = s->Positions[i].Y;
				buf[ind + i].Position.Z = s->Positions[i].Z;
				buf[ind + i].UV.X = s->UV[i].X;
				buf[ind + i].UV.Y = s->UV[i].Y;
				buf[ind + i].Color_.R = s->Colors[i].R;
				buf[ind + i].Color_.G = s->Colors[i].G;
				buf[ind + i].Color_.B = s->Colors[i].B;
				buf[ind + i].Color_.A = s->Colors[i].A;
			}
			ind += 4;
		}

		m_vertexBuffer->Unlock();

		// テクスチャの有無でシェーダーを選択
		std::shared_ptr<NativeShader_Imp> shader;
		if (m_state.TexturePtr != nullptr)
		{
			shader = m_shader;
		}
		else
		{
			shader = m_shader_nt;
		}

		// 描画
		if (m_state.TexturePtr != nullptr)
		{
			shader->SetTexture("g_texture", m_state.TexturePtr, ace::TextureFilterType::Nearest, ace::TextureWrapType::Clamp, 0);
		}
		m_graphics->SetVertexBuffer(m_vertexBuffer.get());
		m_graphics->SetIndexBuffer(m_indexBuffer.get());
		m_graphics->SetShader(shader.get());

		RenderState state;
		
		state.DepthTest = false;
		state.DepthWrite = false;
		state.Culling = CullingType::Double;
		m_graphics->SetRenderState(state);

		m_graphics->DrawPolygon(drawingSprites.size() * 2);

		drawingSprites.clear();
	}
}
