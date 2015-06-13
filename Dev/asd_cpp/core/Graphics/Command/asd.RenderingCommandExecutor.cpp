
#include "asd.RenderingCommandExecutor.h"
#include "asd.RenderingCommand.h"

#include "../asd.Graphics_Imp.h"

#include "../Resource/asd.NativeShader_Imp.h"
#include "../3D/Renderer/asd.SpriteRenderer3D.h"

namespace asd
{
	void RenderingCommandExecutor::Execute(
		Graphics* graphics, 
		Effekseer::Manager* effectManager,
		EffekseerRenderer::Renderer* effectRenderer,
		SpriteRenderer3D* spriteRenerer3D,
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

				if (c->PolyOffset == 0)
				{
					g->DrawPolygon(c->PolyCount);
				}
				else
				{
					g->DrawPolygon(c->PolyOffset, c->PolyCount);
				}
			}
			else if (command->GetType() == RenderingCommandType::DrawInstanced)
			{
				auto c = (RenderingCommand_DrawInstanced*) command;
				c->Shader->SetConstantValues(c->ConstantValues, c->ConstantValueCount);

				g->SetVertexBuffer(c->VB);
				g->SetIndexBuffer(c->IB);
				g->SetShader(c->Shader);
				g->SetRenderState(c->RS);

				g->DrawPolygonInstanced(c->PolyCount, c->InstanceCount);
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
			else if (command->GetType() == RenderingCommandType::DrawSprite)
			{
				auto c = (RenderingCommand_DrawSprite*) command;

				spriteRenerer3D->SetMatrixes(c->CameraMat, c->ProjMat);
				spriteRenerer3D->DrawCache();
			}
		}
	}
}
