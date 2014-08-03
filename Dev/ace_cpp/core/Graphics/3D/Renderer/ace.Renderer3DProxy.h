
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

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
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoBlurXShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoBlurYShader;

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

	public:

		Renderer3DProxy(Graphics* graphics);
		virtual ~Renderer3DProxy();

		void Rendering(RenderTexture2D_Imp* renderTarget);

		Color							SkyAmbientColor;
		Color							GroundAmbientColor;
		RenderSettings					Settings;
		float							DeltaTime;

		void SetEffect(Effekseer::Manager* manager, EffekseerRenderer::Renderer* renderer);
		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);
		
		std::vector<RenderingCommand*>& GetCommands();
		void ResetCommands();
	};
}