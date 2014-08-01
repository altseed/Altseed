
#include "ace.RenderingCommandHelper.h"
#include "ace.RenderingCommandFactory.h"
#include "ace.RenderingCommand.h"
#include "../Resource/ace.NativeShader_Imp.h"

namespace ace
{
	int32_t RenderingCommandHelper::GetConstantBufferID(NativeShader_Imp* shader, const char* name)
	{
		return shader->GetConstantBufferID(name);
	}

	int32_t RenderingCommandHelper::GetTextureID(NativeShader_Imp* shader, const char* name)
	{
		return shader->GetTextureID(name);
	}

	RenderingCommandHelper::RenderingCommandHelper(std::vector<RenderingCommand*>& commands, RenderingCommandFactory* factory)
		: commands(commands)
		, factory(factory)
	{
		
	}

	void RenderingCommandHelper::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
	{
		factory->Clear(commands, isColorTarget, isDepthTarget, color);
	}

	void RenderingCommandHelper::SetRenderTarget(RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth)
	{
		factory->SetRenderTarget(commands, renderTexture0, renderTexture1, renderTexture2, renderTexture3, depth);
	}

	void RenderingCommandHelper::SetRenderTarget(RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth)
	{
		factory->SetRenderTarget(commands, renderTexture0, depth);
	}

	void RenderingCommandHelper::DrawWithPtr(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, ShaderConstantValue* values, int32_t count)
	{
		auto command = factory->CreateCommand<RenderingCommand_Draw>(polyCount, vb, ib, shader, rs);
		command->SetConstantValues(factory, values, count);
		commands.push_back(command);
	}

	void RenderingCommandHelper::Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs)
	{
		auto command = factory->CreateCommand<RenderingCommand_Draw>(polyCount, vb, ib, shader, rs);
		commands.push_back(command);
	}
}