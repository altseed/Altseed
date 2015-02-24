
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.LayerRenderer.h"

#include "../../Log/ace.Log.h"
#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.ShaderCache.h"

#include <Utility/ace.TypeErasureCopy.h>

#include "../Shader/DX/2D/LayerRenderer_PS.h"
#include "../Shader/DX/2D/LayerRenderer_VS.h"
#include "../Shader/GL/2D/LayerRenderer_PS.h"
#include "../Shader/GL/2D/LayerRenderer_VS.h"

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
		vl.push_back(ace::VertexLayout("Pos", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::VertexLayoutFormat::R8G8B8A8_UNORM));

		std::vector<ace::Macro> macro;
		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.LayerRenderer").c_str(),
				layerrenderer_vs_gl,
				layerrenderer_ps_gl,
				vl,
				macro);
		}
		else
		{
			m_shader = m_graphics->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.2D.LayerRenderer").c_str(),
				layerrenderer_vs_dx,
				layerrenderer_ps_dx,
				vl,
				macro);
		}
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
			shader->SetVector2DF("Size", Vector2DF(m_windowSize.X, m_windowSize.Y));
			shader->SetVector4DF("LayerPosU", Vector4DF(m_layerPosition[0].X, m_layerPosition[0].Y, m_layerPosition[1].X, m_layerPosition[1].Y));
			shader->SetVector4DF("LayerPosL", Vector4DF(m_layerPosition[2].X, m_layerPosition[2].Y, m_layerPosition[3].X, m_layerPosition[3].Y));

			// 描画
			if (m_texture != nullptr)
			{
				shader->SetTexture("g_texture", m_texture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp, 0);
			}
			m_graphics->SetVertexBuffer(m_vertexBuffer.get());
			m_graphics->SetIndexBuffer(m_indexBuffer.get());
			m_graphics->SetShader(shader.get());

			RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			state.Culling = CullingType::Double;
			state.AlphaBlendState = AlphaBlend::Blend;
			m_graphics->SetRenderState(state);
			m_graphics->DrawPolygon(vCount / 3);

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

	void LayerRenderer::AddRectangle(Vector2DF positions[4], Color colors[4], Vector2DF uv[4])
	{
		TriangleVertex v[4];
		for (int32_t i = 0; i < 4; i++)
		{
			v[i].Position.X = positions[i].X;
			v[i].Position.Y = positions[i].Y;
			v[i].Position.Z = 0.5f;

			v[i].UV = uv[i];
			v[i].Col = colors[i];
		}

		m_vertecies.push_back(v[0]);
		m_vertecies.push_back(v[1]);
		m_vertecies.push_back(v[2]);

		m_vertecies.push_back(v[0]);
		m_vertecies.push_back(v[2]);
		m_vertecies.push_back(v[3]);
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
