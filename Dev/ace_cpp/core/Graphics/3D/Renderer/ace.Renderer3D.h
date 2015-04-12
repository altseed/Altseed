
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector4DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../../ace.RenderingThread.h"

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

#include <Culling3D.h>

namespace ace
{
	/**
		@brief	レンダラー
		@note
		ライトウエイトレンダラー
		ディレクショナルライト1、半球ライト1のみ、影等なし
	*/
	class Renderer3D
	{
	private:

		class RenderingEvent
			: public RenderingThreadEvent
		{
			Renderer3D*	m_renderer;
		public:
			RenderingEvent(Renderer3D* renderer);
			virtual ~RenderingEvent();
			void Event() override;
		};

		Graphics_Imp*	m_graphics;
		RenderSettings	m_settings;

		std::set<RenderedObject3D*>	objects;
		std::set<RenderedObject3D*>	newObjects;
		std::set<RenderedObject3D*>	removingObjects;

		Color						m_skyAmbientColor;
		Color						m_groundAmbientColor;
		std::shared_ptr<CubemapTexture>	environment_diffuseColor = nullptr;
		std::shared_ptr<CubemapTexture>	environment_specularColor = nullptr;

		Vector2DI					m_windowSize;

		Effekseer::Manager*						m_effectManager = nullptr;
		EffekseerRenderer::Renderer*			m_effectRenderer = nullptr;

		SpriteRenderer3D*						spriteRenderer = nullptr;

		RenderingEvent	m_event;

		RenderingCommandExecutor*				executor = nullptr;
		Renderer3DProxy*						proxy = nullptr;

		bool									hdrMode = false;

		float									ambientColorIntensity = 1.0f;

		float									environmentDiffuseColorIntensity = 1.0f;
		float									environmentSpecularColorIntensity = 1.0f;

	public:

		Renderer3D(Graphics* graphics, RenderSettings settings);
		~Renderer3D();

		RenderSettings GetRenderSettings() const;
		void SetRenderSettings(RenderSettings settings);

		void SetWindowSize(Vector2DI windowSize);

		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);

		void Flip(float deltaTime);

		void BeginRendering(float deltaTime);
		void EndRendering();
		void RenderResult();

		void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D* texture, AlphaBlend alphaBlend, bool depthWrite, bool depthTest);

		float GetAmbientColorIntensity() { return ambientColorIntensity; }
		void SetAmbientColorIntensity(float ambientColorIntensity) { this->ambientColorIntensity = ambientColorIntensity; }

		void SetSkyAmbientColor(Color color) { m_skyAmbientColor = color; }
		void SetGroundAmbientColor(Color color) { m_groundAmbientColor = color; }
	
		void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity)
		{
			this->environmentDiffuseColorIntensity = environmentDiffuseColorIntensity;
			this->environmentSpecularColorIntensity = environmentSpecularColorIntensity;
		}

		void SetEnvironmentColor(CubemapTexture* diffuseColor, CubemapTexture* specularColor);

		float SSAO_Radius = 0.1f;
		float SSAO_Bias = 0.001f;
		float SSAO_Intensity = 1.0f;
		float SSAO_FarPlain = 800.0f;

		Effekseer::Manager*	GetEffectManager() { return m_effectManager; }
	};
}