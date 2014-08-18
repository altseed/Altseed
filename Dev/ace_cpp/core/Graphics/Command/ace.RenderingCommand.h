
#pragma once

#include <ace.common.Base.h>
#include <Math/ace.Matrix44.h>

#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	enum class RenderingCommandType
	{
		Unknown,
		Draw,
		SetRenderTarget,
		Clear,
		DrawEffect,
	};

	class RenderingCommand
	{
	private:
	public:
		RenderingCommand() {}
		virtual ~RenderingCommand() {}
		virtual RenderingCommandType GetType() const = 0;
	};

	struct RenderingCommand_Draw
		: public RenderingCommand
	{
	public:
		RenderingCommandType Type = RenderingCommandType::Draw;

		int32_t				PolyCount = 0;
		VertexBuffer_Imp*	VB = nullptr;
		IndexBuffer_Imp*	IB = nullptr;
		NativeShader_Imp*	Shader = nullptr;
		RenderState			RS = RenderState();

		ShaderConstantValue*	ConstantValues = nullptr;
		int32_t					ConstantValueCount = 0;

		RenderingCommandType GetType() const override { return RenderingCommandType::Draw; }

		RenderingCommand_Draw(int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs);
		virtual ~RenderingCommand_Draw();

		void SetConstantValues(RenderingCommandFactory* factory, ShaderConstantValue* values, int32_t count);
	};

	struct RenderingCommand_SetRenderTarget
		: public RenderingCommand
	{
		RenderTexture2D*	RenderTextures[4];
		DepthBuffer_Imp*	Depth = nullptr;

		RenderingCommandType GetType() const override { return RenderingCommandType::SetRenderTarget; }

		RenderingCommand_SetRenderTarget(RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth);
		RenderingCommand_SetRenderTarget(RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth);
		virtual ~RenderingCommand_SetRenderTarget();
	};

	struct RenderingCommand_Clear
		: public RenderingCommand
	{
		bool	IsColorTarget;
		bool	IsDepthTarget;
		Color	Color_;

		RenderingCommandType GetType() const override { return RenderingCommandType::Clear; }

		RenderingCommand_Clear(bool isColorTarget, bool isDepthTarget, const Color& color);
	};

	struct RenderingCommand_DrawEffect
		: public RenderingCommand
	{
		Matrix44 ProjMat;
		Matrix44 CameraMat;

		RenderingCommandType GetType() const override { return RenderingCommandType::DrawEffect; }

		RenderingCommand_DrawEffect(Matrix44 projMat, Matrix44 cameraMat);
	};
}