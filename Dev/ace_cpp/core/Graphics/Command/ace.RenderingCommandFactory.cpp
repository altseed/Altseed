
#include "ace.RenderingCommandFactory.h"
#include "ace.RenderingCommand.h"

#include <Math/ace.Matrix44.h>

namespace ace
{
	RenderingCommandFactory::RenderingCommandFactory()
	{
		buffer.resize(MainBufferSize);
	}

	RenderingCommandFactory::~RenderingCommandFactory()
	{
		for (auto buf : subBuffers)
		{
			delete [] buf;
		}
		subBuffers.clear();
	}

	void RenderingCommandFactory::Reset()
	{
		std::lock_guard<std::recursive_mutex> lock(mtx_);

		// バッファ追加
		if (subBuffers.size() > 0)
		{
			buffer.resize(buffer.size() + subBuffers.size() * SubBufferSize);

			for (auto buf : subBuffers)
			{
				delete [] buf;
			}
			subBuffers.clear();
		}

		offset = 0;
		subBufferOffset = 0;
	}

	void RenderingCommandFactory::Clear(std::vector<RenderingCommand*>& commands, bool isColorTarget, bool isDepthTarget, const Color& color)
	{
		commands.push_back(CreateCommand<RenderingCommand_Clear>(isColorTarget, isDepthTarget, color));
	}

	void RenderingCommandFactory::SetRenderTarget(std::vector<RenderingCommand*>& commands, RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth)
	{
		commands.push_back(CreateCommand<RenderingCommand_SetRenderTarget>(renderTexture0, renderTexture1, renderTexture2, renderTexture3, depth));
	}

	void RenderingCommandFactory::SetRenderTarget(std::vector<RenderingCommand*>& commands, RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth)
	{
		commands.push_back(CreateCommand<RenderingCommand_SetRenderTarget>(renderTexture0, depth));
	}

	void RenderingCommandFactory::Draw(std::vector<RenderingCommand*>& commands, int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, std::vector<ShaderConstantValue>& constantValues)
	{
		auto command = CreateCommand<RenderingCommand_Draw>(polyCount, vb, ib, shader, rs);
		command->SetConstantValues(this, constantValues.data(), constantValues.size());
		commands.push_back(command);
	}

	void RenderingCommandFactory::DrawEffect(std::vector<RenderingCommand*>& commands, Matrix44 projMat, Matrix44 cameraMat)
	{
		auto command = CreateCommand<RenderingCommand_DrawEffect>(projMat, cameraMat);
		commands.push_back(command);
	}

	void RenderingCommandFactory::DrawSprite(std::vector<RenderingCommand*>& commands, Matrix44 projMat, Matrix44 cameraMat)
	{
		auto command = CreateCommand<RenderingCommand_DrawSprite>(projMat, cameraMat);
		commands.push_back(command);
	}
}