
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

#include "../PostProcess/ace.SSAO.h"
#include "../Object/ace.RenderedObject3D.h"

#include "ace.EnvironmentRendering.h"

namespace ace
{
	class Renderer3DProxy
	{
	private:
		std::shared_ptr<ace::VertexBuffer_Imp>	m_pasteVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_pasteIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_pasteShader;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_shadowVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_shadowIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_shadowShaderX;
		std::shared_ptr<ace::NativeShader_Imp>	m_shadowShaderY;

		std::shared_ptr<ace::NativeShader_Imp>	m_directionalWithAmbientLightShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_directionalLightShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_ambientLightShader;

		std::shared_ptr<ace::NativeShader_Imp>	m_deferredBufferShader;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_ssaoVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_ssaoIndexBuffer;

		std::shared_ptr<RenderTexture2D>		m_shadowTempTexture;

		std::shared_ptr<Texture2D>				dummyTextureWhite;
		std::shared_ptr<Texture2D>				dummyTextureBlack;
		std::shared_ptr<Texture2D>				dummyTextureNormal;

		RenderingCommandFactory*				factory = nullptr;
		std::vector<RenderingCommand*>			commands;

		std::set<RenderedObject3DProxy*>		objects;
		std::set<RenderedObject3DProxy*>		cameraObjects;
		std::set<RenderedObject3DProxy*>		directionalLightObjects;

		Effekseer::Manager*						effectManager = nullptr;
		EffekseerRenderer::Renderer*			effectRenderer = nullptr;

		std::shared_ptr<SSAO>					ssao;
		std::shared_ptr<EnvironmentRendering>	environmentRendering;
	public:

		Renderer3DProxy(Graphics* graphics);
		virtual ~Renderer3DProxy();

		void Rendering(RenderTexture2D_Imp* renderTarget);
		
	private:
		void RenderCamera(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop);
		void RenderCameraOnLightweight(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop);

	public:

		Color							SkyAmbientColor;
		Color							GroundAmbientColor;
		std::shared_ptr<CubemapTexture>	EnvironmentDiffuseColor;
		std::shared_ptr<CubemapTexture>	EnvironmentSpecularColor;
		RenderSettings					Settings;
		float							DeltaTime;

		float SSAO_Radius = 0.1f;
		float SSAO_Bias = 0.001f;
		float SSAO_Intensity = 1.0f;
		float SSAO_FarPlain = 800.0f;

		void SetEffect(Effekseer::Manager* manager, EffekseerRenderer::Renderer* renderer);
		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);
		
		std::vector<RenderingCommand*>& GetCommands();
		void ResetCommands();
	};
}