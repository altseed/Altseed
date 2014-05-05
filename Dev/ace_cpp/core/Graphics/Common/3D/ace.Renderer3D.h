
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector4DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../ace.RenderingThread.h"

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

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

		struct PasteConstantBuffer
		{
			float Size[4];
		};

		struct PasteVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
		};

		struct ShadowBlurConstantBuffer
		{
			Vector4DF	Weights;
		};

		struct ShadowConstantBuffer
		{
			Matrix44	CameraPositionToShadowCameraPosition;
			Matrix44	ShadowProjection;

			float ReconstructInfo1[4];
			float ReconstructInfo2[4];
		};

		struct ShadowVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
		};

		struct SSAOVertex
		{
			Vector3DF	Position;
			Vector2DF	UV;
		};

		struct SSAOConstantVertexBuffer
		{
			float Size[4];
		};

		struct SSAOConstantPixelBuffer
		{
			float Radius;
			float Padding1[3];

			float ProjScale;
			float Padding2[3];

			float Bias;
			float Padding3[3];

			float Intensity;
			float Padding4[3];

			float ReconstructInfo1[4];

			float ReconstructInfo2[4];

		};

		struct
		{
			std::set<RenderedObject3D*>		objects;
			std::set<RenderedObject3D*>		cameraObjects;
			std::set<RenderedObject3D*>		directionalLightObjects;
			Effekseer::Manager*				EffectManager;
			EffekseerRenderer::Renderer*	EffectRenderer;

		} rendering;

		Graphics_Imp*	m_graphics;
		RenderSettings	m_settings;

		std::set<RenderedObject3D*>	m_objects;
		std::set<RenderedObject3D*>	m_cameraObjects;
		std::set<RenderedObject3D*>	m_directionalLightObjects;

		
		bool					m_multithreadingMode;

		Vector2DI				m_windowSize;

		RenderTexture2D_Imp*		m_renderTarget;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_pasteVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_pasteIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_pasteShader;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_shadowVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_shadowIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_shadowShaderX;
		std::shared_ptr<ace::NativeShader_Imp>	m_shadowShaderY;

		std::shared_ptr<ace::NativeShader_Imp>	m_shadowShader;

		std::shared_ptr<ace::VertexBuffer_Imp>	m_ssaoVertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	m_ssaoIndexBuffer;
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoBlurXShader;
		std::shared_ptr<ace::NativeShader_Imp>	m_ssaoBlurYShader;

		std::shared_ptr<RenderTexture2D>		m_shadowTempTexture;

		Effekseer::Manager*						m_effectManager = nullptr;
		EffekseerRenderer::Renderer*			m_effectRenderer = nullptr;

		RenderingEvent	m_event;

	public:
		void Rendering();

		Renderer3D(Graphics* graphics, RenderSettings settings);
		~Renderer3D();

		void SetWindowSize(Vector2DI windowSize);

		void AddObject(RenderedObject3D* o);
		void RemoveObject(RenderedObject3D* o);

		void Flip();

		void BeginRendering();
		void EndRendering();

		RenderTexture2D_Imp* GetRenderTarget();

		Effekseer::Manager*	GetEffectManager() { return m_effectManager; }
	};
}