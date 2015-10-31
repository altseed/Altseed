
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.PostEffectRenderer.h"

#include "../Resource/asd.VertexBuffer_Imp.h"
#include "../Resource/asd.IndexBuffer_Imp.h"
#include "../Resource/asd.Material2D_Imp.h"
#include "../Resource/asd.Shader2D_Imp.h"
#include "../Resource/asd.NativeShader_Imp.h"

#include "../asd.Graphics_Imp.h"

#include "../Command/asd.RenderingCommandHelper.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
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
			vb[0] = Vertex(asd::Vector3DF(-1.0f, 1.0f, 0.5f), asd::Vector2DF(0.0f, 0.0f));
			vb[1] = Vertex(asd::Vector3DF(1.0f, 1.0f, 0.5f), asd::Vector2DF(1.0f, 0.0f));
			vb[2] = Vertex(asd::Vector3DF(1.0f, -1.0f, 0.5f), asd::Vector2DF(1.0f, 1.0f));
			vb[3] = Vertex(asd::Vector3DF(-1.0f, -1.0f, 0.5f), asd::Vector2DF(0.0f, 1.0f));
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
	void PostEffectRenderer::ExportRenderingCommands(RenderingCommandHelper* helper, std::shared_ptr<Material2DCommand> command)
	{
		helper->SetRenderTarget(command->GetTarget(), nullptr);

		// ゴミデータが残っている場合が多いのでリセットする。
		helper->Clear(true, false, Color(0, 0, 0, 0));

		RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;

		helper->DrawWithPtr(2, m_vertexBuffer.get(), m_indexBuffer.get(), command->GetShader(), state, command->GetConstantValues().data(), command->GetConstantValues().size());
	}

	void PostEffectRenderer::DrawOnTexture2DWithMaterialWithCommand(std::shared_ptr<Material2DCommand> command)
	{
		DrawOnTexture2DWithNativeShader(
			(RenderTexture2D_Imp*) command->GetTarget(),
			command->GetShader(),
			command->GetConstantValues().data(),
			command->GetConstantValues().size());
	}
	
	void PostEffectRenderer::DrawOnTexture2DWithNativeShader(RenderTexture2D_Imp* target, NativeShader_Imp* shader, ShaderConstantValue* constantValues, int32_t constantValueCount)
	{
		m_graphics->SetRenderTarget(target, nullptr);

		// ゴミデータが残っている場合が多いのでリセットする。
		m_graphics->Clear(true, false, Color(0, 0, 0, 0));

		shader->SetConstantValues(constantValues, constantValueCount);
		m_graphics->SetVertexBuffer(m_vertexBuffer.get());
		m_graphics->SetIndexBuffer(m_indexBuffer.get());
		m_graphics->SetShader(shader);

		RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;

		// インテルやMacLightBloomで画面が崩れるのを回避。原因は不明(おそらくドライバの問題)
		state.AlphaBlendState = AlphaBlendMode::Opacity;

		m_graphics->SetRenderState(state);
		m_graphics->DrawPolygon(2);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}