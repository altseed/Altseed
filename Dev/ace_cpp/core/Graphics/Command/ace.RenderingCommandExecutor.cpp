
#include "ace.RenderingCommandExecutor.h"
#include "ace.RenderingCommand.h"

#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.NativeShader_Imp.h"

namespace ace
{
	void RenderingCommandExecutor::Execute(
		Graphics* graphics, 
		Effekseer::Manager* effectManager,
		EffekseerRenderer::Renderer* effectRenderer,
		std::vector<RenderingCommand*>& commands)
	{
		auto commands_ptr = commands.data();
		auto commands_size = commands.size();
		auto g = (Graphics_Imp*) graphics;

		for (int32_t i = 0; i < commands_size; i++)
		{
			auto& command = commands_ptr[i];

			if (command->GetType() == RenderingCommandType::Clear)
			{
				auto c = (RenderingCommand_Clear*) command;
				g->Clear(c->IsColorTarget, c->IsDepthTarget, c->Color_);
			}
			else if (command->GetType() == RenderingCommandType::Draw)
			{
				auto c = (RenderingCommand_Draw*) command;
				c->Shader->SetConstantValues(c->ConstantValues, c->ConstantValueCount);
				
				g->SetVertexBuffer(c->VB);
				g->SetIndexBuffer(c->IB);
				g->SetShader(c->Shader);
				g->SetRenderState(c->RS);
				g->DrawPolygon(c->PolyCount);
			}
			else if (command->GetType() == RenderingCommandType::SetRenderTarget)
			{
				auto c = (RenderingCommand_SetRenderTarget*) command;

				if (c->RenderTextures[1] == nullptr && c->RenderTextures[2] == nullptr && c->RenderTextures[2] == nullptr)
				{
					g->SetRenderTarget(
						(RenderTexture2D_Imp*)c->RenderTextures[0],
						c->Depth);
				}
				else
				{
					g->SetRenderTarget(
						(RenderTexture2D_Imp*) c->RenderTextures[0],
						(RenderTexture2D_Imp*) c->RenderTextures[1],
						(RenderTexture2D_Imp*) c->RenderTextures[2],
						(RenderTexture2D_Imp*) c->RenderTextures[3],
						c->Depth);
				}
			}
			else if (command->GetType() == RenderingCommandType::DrawEffect)
			{
				auto c = (RenderingCommand_DrawEffect*) command;

				// 行列を転置して設定
				Effekseer::Matrix44 cameraMat, projMat;
				for (auto c_ = 0; c_ < 4; c_++)
				{
					for (auto r = 0; r < 4; r++)
					{
						cameraMat.Values[c_][r] = c->CameraMat.Values[r][c_];
						projMat.Values[c_][r] = c->ProjMat.Values[r][c_];
					}
				}
				effectRenderer->SetCameraMatrix(cameraMat);
				effectRenderer->SetProjectionMatrix(projMat);
				effectRenderer->BeginRendering();
				effectManager->Draw();
				effectRenderer->EndRendering();

				// レンダー設定リセット
				g->CommitRenderState(true);
			}
		}
	}
}
