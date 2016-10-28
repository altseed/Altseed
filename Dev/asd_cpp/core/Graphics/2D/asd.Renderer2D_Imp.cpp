
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Renderer2D_Imp.h"

#include "../../Log/asd.Log.h"
#include "../asd.Graphics_Imp.h"
#include "../Resource/asd.VertexBuffer_Imp.h"
#include "../Resource/asd.IndexBuffer_Imp.h"
#include "../Resource/asd.NativeShader_Imp.h"

#include "../Resource/asd.Shader2D_Imp.h"
#include "../Resource/asd.Material2D_Imp.h"
#include "../Resource/asd.ShaderCache.h"

#include "../Resource/asd.Font_Imp.h"
#include "../Resource/asd.Texture2D_Imp.h"

#include <Utility/asd.TypeErasureCopy.h>

#include "../Shader/DX/2D/Renderer2D_PS.h"
#include "../Shader/DX/2D/Renderer2D_VS.h"
#include "../Shader/GL/2D/Renderer2D_PS.h"
#include "../Shader/GL/2D/Renderer2D_VS.h"

#if _WIN32
#include "../Platform/DX11/asd.Graphics_Imp_DX11.h"
#endif
#include "../Platform/GL/asd.Graphics_Imp_GL.h"

#ifdef WIN32
#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

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
	Renderer2D_Imp::Renderer2D_Imp(Graphics* graphics, Log* log)
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

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Pos", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("Color", asd::VertexLayoutFormat::R8G8B8A8_UNORM));

		std::vector<asd::Macro> macro_tex;
		macro_tex.push_back(Macro("HAS_TEXTURE", "1"));

		std::vector<asd::Macro> macro;

		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
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

		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
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

		// エフェクト
		{
			m_effectManager = ::Effekseer::Manager::Create(6000, false);
			if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
			{
#if _WIN32
				auto g = (Graphics_Imp_DX11*) m_graphics;
				m_effectRenderer = ::EffekseerRendererDX11::Renderer::Create(g->GetDevice(), g->GetContext(), 6000);

				auto distortion = new DistortingCallbackDX11(
					(::EffekseerRendererDX11::Renderer*)m_effectRenderer,
					g->GetDevice(),
					g->GetContext());

				m_effectRenderer->SetDistortingCallback(distortion);
#endif
			}
			else if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_effectRenderer = ::EffekseerRendererGL::Renderer::Create(6000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);

				auto distortion = new DistortingCallbackGL(
					(::EffekseerRendererGL::Renderer*)m_effectRenderer);

				m_effectRenderer->SetDistortingCallback(distortion);
			}

			m_effectManager->SetSpriteRenderer(m_effectRenderer->CreateSpriteRenderer());
			m_effectManager->SetRibbonRenderer(m_effectRenderer->CreateRibbonRenderer());
			m_effectManager->SetRingRenderer(m_effectRenderer->CreateRingRenderer());
			m_effectManager->SetModelRenderer(m_effectRenderer->CreateModelRenderer());
			m_effectManager->SetTrackRenderer(m_effectRenderer->CreateTrackRenderer());

			m_effectManager->SetSetting(m_graphics->GetEffectSetting());
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

		m_effectRenderer->Destory();
		m_effectManager->Destroy();
		m_effectRenderer = nullptr;
		m_effectManager = nullptr;

		SafeRelease(m_graphics);
	}

	void Renderer2D_Imp::SetArea(const RectF& area, float angle)
	{
		this->area = area;
		this->angle = angle;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::DrawCache()
	{
		auto ang = angle;

		// エフェクト設定
		{
			Matrix44 effectProjMat;
			Matrix44 effectCameraMat;
			effectProjMat.SetOrthographicRH(area.Width, area.Height, 0.1, 800.0);

			auto px = area.X + area.Width / 2;
			auto py = -(area.Y + area.Height / 2);

			Vector3DF up = Vector3DF(sin(ang), cos(ang), 0);

			effectCameraMat.SetLookAtRH(Vector3DF(px, py, 400), Vector3DF(px, py, 0), up);

			// 行列を転置して設定

			Effekseer::Matrix44 cameraMat, projMat;
			for (auto c_ = 0; c_ < 4; c_++)
			{
				for (auto r = 0; r < 4; r++)
				{
					cameraMat.Values[c_][r] = effectCameraMat.Values[r][c_];
					projMat.Values[c_][r] = effectProjMat.Values[r][c_];
				}
			}
			m_effectRenderer->SetCameraMatrix(cameraMat);
			m_effectRenderer->SetProjectionMatrix(projMat);
		}

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
				if (e.Type == Event::EventType::Sprite)
				{
					SafeRelease(e.Data.Sprite.TexturePtr);
					SafeRelease(e.Data.Sprite.Material2DPtr);
				}
				else if (e.Type == Event::EventType::Effect)
				{
				}
			}

			c.second.clear();
		}
	}

	void Renderer2D_Imp::AddSpriteWithMaterial(Vector2DF positions[4], Color colors[4], Vector2DF uv[4], Material2D* material, AlphaBlendMode alphaBlend, int32_t priority)
	{
		Event e;
		e.Type = Event::EventType::Sprite;

		memcpy(e.Data.Sprite.Positions, positions, sizeof(asd::Vector2DF) * 4);
		memcpy(e.Data.Sprite.Colors, colors, sizeof(asd::Color) * 4);
		memcpy(e.Data.Sprite.UV, uv, sizeof(asd::Vector2DF) * 4);
		e.Data.Sprite.AlphaBlendState = alphaBlend;
		e.Data.Sprite.TexturePtr = nullptr;
		e.Data.Sprite.Material2DPtr = material;

		SafeAddRef(e.Data.Sprite.TexturePtr);
		SafeAddRef(e.Data.Sprite.Material2DPtr);

		AddEvent(priority, e);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::AddSprite(Vector2DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority, TextureFilterType filter, TextureWrapType wrap)
	{
		Event e;
		e.Type = Event::EventType::Sprite;

		memcpy(e.Data.Sprite.Positions, positions, sizeof(asd::Vector2DF) * 4);
		memcpy(e.Data.Sprite.Colors, colors, sizeof(asd::Color) * 4);
		memcpy(e.Data.Sprite.UV, uv, sizeof(asd::Vector2DF) * 4);
		e.Data.Sprite.AlphaBlendState = alphaBlend;
		e.Data.Sprite.TexturePtr = texture;
		e.Data.Sprite.Material2DPtr = nullptr;
		e.Data.Sprite.Filter = filter;
		e.Data.Sprite.Wrap = wrap;

		SafeAddRef(e.Data.Sprite.TexturePtr);
		SafeAddRef(e.Data.Sprite.Material2DPtr);

		AddEvent(priority, e);
	}

	void Renderer2D_Imp::AddText(Matrix33& parentMatrix, Matrix33& matrix, Vector2DF centerPosition, bool turnLR, bool turnUL, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority, float lineSpacing, float letterSpacing, TextureFilterType filter, TextureWrapType wrap)
	{
		Vector2DF drawPosition = Vector2DF(0, 0);

		std::array<Color, 4> colors;
		colors.at(0) = color;
		colors.at(1) = color;
		colors.at(2) = color;
		colors.at(3) = color;

		float offset = 0;
		Font_Imp* font_Imp = (Font_Imp*)font;

		font_Imp->AddCharactorsDynamically(text);
		font_Imp->UpdateTextureDynamically();

		for (int textIndex = 0;; ++textIndex)
		{
			if (text[textIndex] == 0) break;

			if (text[textIndex] != '\n' && !font_Imp->HasGlyphData(text[textIndex]))
			{
				continue;
			}
			else if (text[textIndex] == '\n')
			{
				if (writingDirection == WritingDirection::Horizontal)
				{
					drawPosition.X = 0;
					drawPosition.Y += (offset + lineSpacing);
				}
				else
				{
					drawPosition.X += (offset + lineSpacing);
					drawPosition.Y = 0;
				}
				offset = 0;

				continue;
			}

			const GlyphData glyphData = font_Imp->GetGlyphData(text[textIndex]);
			auto texture = font_Imp->GetTexture(glyphData.GetSheetNum());

			if (texture == nullptr)
			{
				continue;
			}

			const auto glyphSrc = glyphData.GetSrc();

			std::array<Vector2DF, 4> position;

			{
				position.at(0) = Vector2DF(0, 0);
				position.at(1) = Vector2DF(glyphSrc.Width, 0);
				position.at(2) = Vector2DF(glyphSrc.Width, glyphSrc.Height);
				position.at(3) = Vector2DF(0, glyphSrc.Height);

				for (auto& pos : position)
				{
					pos += drawPosition;
					pos -= centerPosition;
					auto v3 = Vector3DF(pos.X, pos.Y, 1);
					auto result = parentMatrix * matrix * v3;
					pos = Vector2DF(result.X, result.Y);
				}

			}

			std::array<Vector2DF, 4> uvs;
			{
				const auto textureSize = Vector2DF(texture->GetSize().X, texture->GetSize().Y);

				uvs.at(0) = Vector2DF(glyphSrc.X, glyphSrc.Y);
				uvs.at(1) = Vector2DF(glyphSrc.X + glyphSrc.Width, glyphSrc.Y);
				uvs.at(2) = Vector2DF(glyphSrc.X + glyphSrc.Width, glyphSrc.Y + glyphSrc.Height);
				uvs.at(3) = Vector2DF(glyphSrc.X, glyphSrc.Y + glyphSrc.Height);

				for (auto& uv : uvs)
				{
					uv /= textureSize;
				}

				if (turnLR)
				{
					std::swap(uvs.at(0), uvs.at(1));
					std::swap(uvs.at(2), uvs.at(3));
				}

				if (turnUL)
				{
					std::swap(uvs.at(0), uvs.at(3));
					std::swap(uvs.at(1), uvs.at(2));
				}
			}

			AddSprite(position.data(), &colors[0], uvs.data(), texture.get(), alphaBlend, priority, filter, wrap);

			if (writingDirection == WritingDirection::Horizontal)
			{
				drawPosition += asd::Vector2DF(glyphSrc.Width + letterSpacing, 0);
				offset = std::max((float)glyphSrc.Height, offset);
			}
			else
			{
				drawPosition += asd::Vector2DF(0, glyphSrc.Height + letterSpacing);
				offset = std::max((float)glyphSrc.Height, offset);
			}
		}
	}

	void Renderer2D_Imp::AddEffect(::Effekseer::Handle handle, int32_t priority)
	{
		Event e;
		e.Type = Event::EventType::Effect;
		e.Data.Effect.EffectHandle = handle;

		AddEvent(priority, e);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::AddEvent(int32_t priority, Event& e)
	{
		auto ev = m_events.find(priority);
		if (ev == m_events.end())
		{
			m_events[priority] = std::vector<Event>({e});
		}
		else
		{
			(*ev).second.push_back(e);
		}
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
			m_state.TexturePtr = e.Data.Sprite.TexturePtr;
			m_state.Material2DPtr = e.Data.Sprite.Material2DPtr;
			m_state.AlphaBlendState = e.Data.Sprite.AlphaBlendState;
			m_state.Filter = e.Data.Sprite.Filter;
			m_state.Wrap = e.Data.Sprite.Wrap;
		};

		if (e.Type == Event::EventType::Sprite)
		{
			DrawEffect();

			if (m_drawingSprites.size() == 0)
			{
				// 初期値設定
				resetState();
			}
			else
			{
				// 同時描画不可のケースかどうか?
				// もしくはバッファが溢れないかどうか?
				if (m_state.TexturePtr != e.Data.Sprite.TexturePtr ||
					m_state.Material2DPtr != e.Data.Sprite.Material2DPtr ||
					m_state.AlphaBlendState != e.Data.Sprite.AlphaBlendState ||
					m_state.Filter != e.Data.Sprite.Filter ||
					m_state.Wrap != e.Data.Sprite.Wrap ||

					m_drawingSprites.size() >= SpriteCount)
				{
					DrawSprite();
					resetState();
				}
			}

			// 書き込み
			m_drawingSprites.push_back(&e);
		}
		else if (e.Type == Event::EventType::Effect)
		{
			DrawSprite();

			// 書き込み
			drawingEffects.push_back(&e);
		}
		else
		{
			DrawSprite();
			DrawEffect();
		}

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::EndDrawing()
	{
		DrawSprite();
		DrawEffect();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Renderer2D_Imp::DrawSprite()
	{
		if (m_drawingSprites.size() == 0) return;

		auto isMaterialMode = m_state.Material2DPtr != nullptr;

		// 行列を計算
		Matrix44 mat, mat_t, mat_scale, mat_rot;

		mat_t.SetTranslation(-(area.X + area.Width / 2.0f), -(area.Y + area.Height / 2.0f), 0);
		mat_scale.SetScale(2.0f / area.Width, -2.0f / area.Height, 1.0f);
		mat_rot.SetRotationZ(angle);

		mat = mat_rot * mat_scale * mat_t;

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

			// マテリアルモードではシェーダーで行列計算をしないため、CPUで計算
			if (isMaterialMode)
			{
				for (int32_t i = 0; i < 4; i++)
				{
					buf[ind + i].Position = mat.Transform3D(buf[ind + i].Position);
				}
			}
			ind += 4;
		}

		m_vertexBuffer->Unlock();

		NativeShader_Imp* shader = nullptr;

		// テクスチャの有無でシェーダーを選択
		if (isMaterialMode)
		{
			auto material = (Material2D_Imp*) m_state.Material2DPtr;
			auto command = material->GenerateShaderCommand();
			
			shader = command->GetShader();
			auto& constantValues = command->GetConstantValues();
			shader->SetConstantValues(constantValues.data(), constantValues.size());
		}
		else
		{
			if (m_state.TexturePtr != nullptr)
			{
				shader = m_shader.get();
			}
			else
			{
				shader = m_shader_nt.get();
			}
			shader->SetMatrix44("mat", mat);
		}


		// 描画
		if (m_state.Material2DPtr == nullptr && m_state.TexturePtr != nullptr)
		{
			shader->SetTexture("g_texture", m_state.TexturePtr, m_state.Filter, m_state.Wrap, 0);
		}
		m_graphics->SetVertexBuffer(m_vertexBuffer.get());
		m_graphics->SetIndexBuffer(m_indexBuffer.get());
		m_graphics->SetShader(shader);

		RenderState state;
		
		state.AlphaBlendState = m_state.AlphaBlendState;
		state.DepthTest = false;
		state.DepthWrite = false;
		state.Culling = CullingType::Double;
		m_graphics->SetRenderState(state);

		m_graphics->DrawPolygon(m_drawingSprites.size() * 2);

		m_drawingSprites.clear();
	}


	void Renderer2D_Imp::DrawEffect()
	{
		if (drawingEffects.size() == 0) return;

		m_effectRenderer->BeginRendering();
		
		for (auto& e : drawingEffects)
		{
			m_effectManager->DrawHandle(e->Data.Effect.EffectHandle);
		}

		m_effectRenderer->EndRendering();

		// レンダー設定リセット
		m_graphics->CommitRenderState(true);

		drawingEffects.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
