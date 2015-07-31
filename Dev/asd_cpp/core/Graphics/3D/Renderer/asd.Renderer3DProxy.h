
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

#include "../PostProcess/asd.SSAO.h"
#include "../Object/asd.RenderedObject3D.h"

#include "asd.EnvironmentRendering.h"
#include "asd.HeightFogRendering.h"

#include <Culling3D.h>

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/Culling3D.lib")
#else
#pragma comment(lib,"x64/Release/Culling3D.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/Culling3D.lib")
#else
#pragma comment(lib,"x86/Release/Culling3D.lib")
#endif

#endif

namespace asd
{
	class Renderer3DProxy
	{
	private:
		template<class T>
		void DrawObjects(T& objects_, RenderingCommandHelper* helper, RenderingProperty prop)
		{
			for (auto& o : objects_)
			{
				o->Rendering(helper, prop);
			}
		}

		void DrawMassObjects(RenderingCommandHelper* helper, RenderingProperty prop);

		template<class T>
		void SortAndSetMassObjects(T& objects_)
		{
			sortedMassModelObjects.clear();

			for (auto& o : objects_)
			{
				sortedMassModelObjects.push_back((RenderedMassModelObject3DProxy*) o);
			}
			SortAndSetMassObjects_Imp();
		}
		void SortAndSetMassObjects_Imp();

	private:
		Graphics*								graphics = nullptr;

		std::shared_ptr<asd::VertexBuffer_Imp>	m_pasteVertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	m_pasteIndexBuffer;
		std::shared_ptr<asd::NativeShader_Imp>	m_pasteShader;

		std::shared_ptr<asd::VertexBuffer_Imp>	m_shadowVertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	m_shadowIndexBuffer;
		std::shared_ptr<asd::NativeShader_Imp>	m_shadowShaderX;
		std::shared_ptr<asd::NativeShader_Imp>	m_shadowShaderY;

		std::shared_ptr<asd::NativeShader_Imp>	m_directionalWithAmbientLightShader;
		std::shared_ptr<asd::NativeShader_Imp>	m_directionalLightShader;
		std::shared_ptr<asd::NativeShader_Imp>	m_ambientLightShader;

		std::shared_ptr<asd::NativeShader_Imp>	m_deferredBufferShader;

		std::shared_ptr<asd::VertexBuffer_Imp>	m_ssaoVertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	m_ssaoIndexBuffer;

		std::shared_ptr<RenderTexture2D>		m_shadowTempTexture;

		std::shared_ptr<Texture2D>				dummyTextureWhite;
		std::shared_ptr<Texture2D>				dummyTextureBlack;
		std::shared_ptr<Texture2D>				dummyTextureNormal;

		RenderingCommandFactory*				factory = nullptr;
		std::vector<RenderingCommand*>			commands;

		std::set<RenderedObject3DProxy*>		objects;
		std::set<RenderedObject3DProxy*>		massModelObjects;
		std::set<RenderedObject3DProxy*>		cameraObjects;
		std::set<RenderedObject3DProxy*>		directionalLightObjects;
		
		std::vector<RenderedMassModelObject3DProxy*>		sortedMassModelObjects;

		Effekseer::Manager*						effectManager = nullptr;
		EffekseerRenderer::Renderer*			effectRenderer = nullptr;

		std::shared_ptr<SSAO>					ssao;
		std::shared_ptr<EnvironmentRendering>	environmentRendering;
		std::shared_ptr<HeightFogRendering>		heightfogRendering;
	public:

		Renderer3DProxy(Graphics* graphics);
		virtual ~Renderer3DProxy();

		/**
			@brief	内部の情報から現在のフレームを内部に描画する。
		*/
		void Render();
		
		/**
			@brief	内部の描画結果を現在のレンダーターゲットに描画する。
		*/
		void RenderResult();

	private:
		void RenderCamera(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop);
		void RenderCameraOnLightweight(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop);

	private:
		std::vector<RenderedObject3DProxy*>				culledObjects;
		std::vector<RenderedObject3DCullingProxy*>		culledTerrainObjects;
		std::vector<RenderedObject3DProxy*>				culledMassModelObjects;

		void Culling(const Matrix44& viewProjectionMat);

	private:
		
	public:

		Culling3D::World*				CullingWorld = nullptr;

		Color							SkyAmbientColor;
		Color							GroundAmbientColor;
		float							AmbientColorIntensity = 1.0f;

		std::shared_ptr<CubemapTexture>	EnvironmentDiffuseColor;
		std::shared_ptr<CubemapTexture>	EnvironmentSpecularColor;

		float							EnvironmentDiffuseColorIntensity = 1.0f;
		float							EnvironmentSpecularColorIntensity = 1.0f;


		RenderSettings					Settings;
		float							DeltaTime;
		bool							HDRMode = false;

		float SSAO_Radius = 0.1f;
		float SSAO_Bias = 0.001f;
		float SSAO_Intensity = 1.0f;
		float SSAO_FarPlain = 800.0f;

		bool IsHeightFogEnabled = false;
		float HeightFogDensity = 1.0f;
		Color HeightFogColor = Color(255, 255, 255, 255);
		float HeightFogFalloff = 10.0f;
		float HeightFogStartDistance = 1.0f;

		void SetEffect(Effekseer::Manager* manager, EffekseerRenderer::Renderer* renderer);
		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);
		
		std::vector<RenderingCommand*>& GetCommands();
		void ResetCommands();
	};
}