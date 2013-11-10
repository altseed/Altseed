
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.PostEffectRenderer.h"

#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"
#include "../Resource/ace.Material2D_Imp.h"
#include "../Resource/ace.Shader2D_Imp.h"

#include "../ace.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	PostEffectRenderer::PostEffectRenderer(
		Graphics* graphics,
		std::shared_ptr<VertexBuffer_Imp> vertexBuffer,
		std::shared_ptr<IndexBuffer_Imp> indexBuffer)
		: m_graphics(nullptr)
	{
		m_graphics = (Graphics_Imp*)graphics;
		m_vertexBuffer = vertexBuffer;
		m_indexBuffer = indexBuffer;

		SafeAddRef(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	PostEffectRenderer::~PostEffectRenderer()
	{
		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	PostEffectRenderer* PostEffectRenderer::Create(Graphics* graphics)
	{
		std::shared_ptr<VertexBuffer_Imp> vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer_Imp> indexBuffer = nullptr;

		auto g = (Graphics_Imp*) graphics;

		vertexBuffer = g->CreateVertexBuffer_Imp(sizeof(Vertex), 4, false);
		indexBuffer = g->CreateIndexBuffer_Imp(6, false, false);
	
		if (vertexBuffer == nullptr ||
			indexBuffer == nullptr)
		{
			return nullptr;
		}

		{
			vertexBuffer->Lock();
			auto vb = vertexBuffer->GetBuffer<Vertex>(4);
			vb[0] = Vertex(ace::Vector3DF(-1.0f, 1.0f, 0.5f), ace::Vector2DF(0.0f, 0.0f));
			vb[1] = Vertex(ace::Vector3DF(1.0f, 1.0f, 0.5f), ace::Vector2DF(1.0f, 0.0f));
			vb[2] = Vertex(ace::Vector3DF(1.0f, -1.0f, 0.5f), ace::Vector2DF(1.0f, 1.0f));
			vb[3] = Vertex(ace::Vector3DF(-1.0f, -1.0f, 0.5f), ace::Vector2DF(0.0f, 1.0f));
			vertexBuffer->Unlock();
		}

		{
			indexBuffer->Lock();
			auto ib = indexBuffer->GetBuffer<uint16_t>(6);
			ib[0] = 0;
			ib[1] = 1;
			ib[2] = 2;
			ib[3] = 0;
			ib[4] = 2;
			ib[5] = 3;
			indexBuffer->Unlock();
		}

		return new PostEffectRenderer(graphics, vertexBuffer, indexBuffer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void PostEffectRenderer::DrawOnTexture2DWithMaterial_Imp(RenderTexture_Imp* target, Material2D_Imp* material)
	{
		auto state = m_graphics->GetRenderState()->Push();

		material->SetValuesToShader();

		m_graphics->SetRenderTarget(target, nullptr);

		m_graphics->SetVertexBuffer(m_vertexBuffer.get());
		m_graphics->SetIndexBuffer(m_indexBuffer.get());
		m_graphics->SetShader(material->GetShader_Imp()->GetNativeShader().get());
		
		state.DepthTest = false;
		state.DepthWrite = false;
		m_graphics->GetRenderState()->Update(false);

		m_graphics->DrawPolygon(2);

		m_graphics->GetRenderState()->Pop();
	}
	

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}