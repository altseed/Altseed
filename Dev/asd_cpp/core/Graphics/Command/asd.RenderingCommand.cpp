
#include "asd.RenderingCommand.h"
#include "asd.RenderingCommandFactory.h"

#include "../Resource/asd.VertexBuffer_Imp.h"
#include "../Resource/asd.IndexBuffer_Imp.h"
#include "../Resource/asd.NativeShader_Imp.h"
#include "../Resource/asd.RenderTexture2D_Imp.h"
#include "../Resource/asd.DepthBuffer_Imp.h"

namespace asd
{
	RenderingCommand_Draw::RenderingCommand_Draw(int32_t polyOffset, int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs)
		: PolyOffset(polyOffset)
		, PolyCount(polyCount)
		, VB(vb)
		, IB(ib)
		, Shader(shader)
	{
		SafeAddRef(vb);
		SafeAddRef(ib);
		SafeAddRef(shader);

		RS = rs;
	}

	RenderingCommand_Draw::RenderingCommand_Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs)
		: PolyCount(polyCount)
		, VB(vb)
		, IB(ib)
		, Shader(shader)
	{
		SafeAddRef(vb);
		SafeAddRef(ib);
		SafeAddRef(shader);

		RS = rs;
	}

	RenderingCommand_Draw::~RenderingCommand_Draw()
	{
		for (auto i = 0; i < ConstantValueCount; i++)
		{
			ConstantValues[i].~ShaderConstantValue();
		}

		SafeRelease(VB);
		SafeRelease(IB);
		SafeRelease(Shader);
	}

	void RenderingCommand_Draw::SetConstantValues(RenderingCommandFactory* factory, ShaderConstantValue* values, int32_t count)
	{
		ConstantValues = factory->CreateCommands<ShaderConstantValue>(count);
		ConstantValueCount = count;

		for (auto i = 0; i < ConstantValueCount; i++)
		{
			ConstantValues[i] = values[i];
		}
	}

	RenderingCommand_DrawInstanced::RenderingCommand_DrawInstanced(int32_t polyCount, int32_t instanceCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs)
		: PolyCount(polyCount)
		, InstanceCount(instanceCount)
		, VB(vb)
		, IB(ib)
		, Shader(shader)
	{
		SafeAddRef(vb);
		SafeAddRef(ib);
		SafeAddRef(shader);

		RS = rs;
	}

	RenderingCommand_DrawInstanced::~RenderingCommand_DrawInstanced()
	{
		for (auto i = 0; i < ConstantValueCount; i++)
		{
			ConstantValues[i].~ShaderConstantValue();
		}

		SafeRelease(VB);
		SafeRelease(IB);
		SafeRelease(Shader);
	}

	void RenderingCommand_DrawInstanced::SetConstantValues(RenderingCommandFactory* factory, ShaderConstantValue* values, int32_t count)
	{
		ConstantValues = factory->CreateCommands<ShaderConstantValue>(count);
		ConstantValueCount = count;

		for (auto i = 0; i < ConstantValueCount; i++)
		{
			ConstantValues[i] = values[i];
		}
	}

	RenderingCommand_SetRenderTarget::RenderingCommand_SetRenderTarget(RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth)
	{
		RenderTextures[0] = renderTexture0;
		RenderTextures[1] = renderTexture1;
		RenderTextures[2] = renderTexture2;
		RenderTextures[3] = renderTexture3;
		Depth = depth;

		for (auto i = 0; i < 4; i++)
		{
			SafeAddRef(RenderTextures[i]);
		}
		SafeAddRef(Depth);
	}

	RenderingCommand_SetRenderTarget::RenderingCommand_SetRenderTarget(RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth)
	{
		RenderTextures[0] = renderTexture0;
		RenderTextures[1] = nullptr;
		RenderTextures[2] = nullptr;
		RenderTextures[3] = nullptr;
		Depth = depth;

		SafeAddRef(RenderTextures[0]);
		SafeAddRef(Depth);
	}

	RenderingCommand_SetRenderTarget::~RenderingCommand_SetRenderTarget()
	{
		for (auto i = 0; i < 4; i++)
		{
			SafeRelease(RenderTextures[i]);
		}
		SafeRelease(Depth);
	}

	RenderingCommand_Clear::RenderingCommand_Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
	{
		IsColorTarget = isColorTarget;
		IsDepthTarget = isDepthTarget;
		Color_ = color;
	}

	RenderingCommand_DrawEffect::RenderingCommand_DrawEffect(Matrix44 projMat, Matrix44 cameraMat)
		: ProjMat(projMat)
		, CameraMat(cameraMat)
	{
	}

	RenderingCommand_DrawSprite::RenderingCommand_DrawSprite(Matrix44 projMat, Matrix44 cameraMat)
		: ProjMat(projMat)
		, CameraMat(cameraMat)
	{
	}
}