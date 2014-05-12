
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector3DF.h>

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedCameraObject3D
		: public RenderedObject3D
	{
	private:

		struct
		{
			float	zfar;
			float	znear;
			float	fov;

			Vector2DI	size;
			Vector3DF	focus;

			int32_t	postEffectCount;

		} m_values;

		struct
		{
			float	zfar;
			float	znear;
			float	fov;
			Vector2DI	size;
			Vector3DF	focus;
			Matrix44	cameraMatrix;
			Matrix44	projectionMatrix;

			int32_t	postEffectCount;

		} m_values_RT;

		RenderTexture2D_Imp*	m_renderTargetDiffuseColor_RT = nullptr;
		RenderTexture2D_Imp*	m_renderTargetSpecularColor_Smoothness_RT = nullptr;
		RenderTexture2D_Imp*	m_renderTargetNormalDepth_RT = nullptr;
		RenderTexture2D_Imp*	m_renderTargetAO_MatID_RT = nullptr;

		RenderTexture2D_Imp*	m_renderTargetSSAO_RT;
		RenderTexture2D_Imp*	m_renderTargetSSAO_temp_RT;
		RenderTexture2D_Imp*	m_renderTargetShadow_RT;

		RenderTexture2D_Imp*	m_renderTarget_FR[2];
		DepthBuffer_Imp*	m_depthBuffer_RT;

		std::vector<std::shared_ptr<Material2DCommand>>	m_postEffectCommands;
		std::vector<std::shared_ptr<Material2DCommand>>	m_postEffectCommands_RT;

		PostEffectRenderer*								m_postEffectRenderer;

	public:
		RenderedCameraObject3D(Graphics* graphics);
		virtual ~RenderedCameraObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		Vector2DI GetWindowSize() const { return m_values.size; }
		void SetWindowSize( const Vector2DI& size );

		Vector3DF GetFocus(){ return m_values.focus; }
		void SetFocus(const Vector3DF& focus);

		float GetFieldOfView(){ return m_values.fov; }
		void SetFieldOfView(float fov);

		float GetZFar() { return m_values.zfar; }
		void SetZFar(float zfar);

		float GetZNear(){ return m_values.znear; }
		void SetZNear(float znear);

		void SetPostEffectCount(int32_t postEffectCount);
		void AddPostEffectCommand(std::shared_ptr<Material2DCommand> command);

		/**
			@brief	ポストエフェクト設定時の出力先を取得する。
			@param	count	現在の描画回数
			@note
			別スレッドで描画に使用されている可能性が高いので注意する。
		*/
		RenderTexture2D* GetDstForPostEffect(int32_t count);

		/**
		@brief	ポストエフェクト設定時の出力元を取得する。
		@param	count	現在の描画回数
		@note
		別スレッドで描画に使用されている可能性が高いので注意する。
		*/
		RenderTexture2D* GetSrcForPostEffect(int32_t count);

		void CalculateMatrix_RT() override;

		void ApplyPostEffects_RT();

		RenderTexture2D_Imp* GetRenderTarget_RT();
		RenderTexture2D_Imp* GetAffectedRenderTarget_RT();

		DepthBuffer_Imp* GetDepthBuffer_RT() { return m_depthBuffer_RT; }

		RenderTexture2D_Imp*	GetRenderTargetDiffuseColor_RT() { return m_renderTargetDiffuseColor_RT; }
		RenderTexture2D_Imp*	GetRenderTargetSpecularColor_Smoothness_RT() { return m_renderTargetSpecularColor_Smoothness_RT; }
		RenderTexture2D_Imp*	GetRenderTargetDepth_RT() { return m_renderTargetNormalDepth_RT; }
		RenderTexture2D_Imp*	GetRenderTargetAO_MatID_RT() { return m_renderTargetAO_MatID_RT; }


		RenderTexture2D_Imp*	GetRenderTargetShadow_RT() { return m_renderTargetShadow_RT; }

		RenderTexture2D_Imp*	GetRenderTargetSSAO_RT() { return m_renderTargetSSAO_RT; }
		RenderTexture2D_Imp*	GetRenderTargetSSAO_Temp_RT() { return m_renderTargetSSAO_temp_RT; }

		const Matrix44& GetCameraMatrix_RT() { return m_values_RT.cameraMatrix; }
		const Matrix44& GetProjectionMatrix_RT() { return m_values_RT.projectionMatrix; }

		Vector3DF GetFocus_RT(){ return m_values_RT.focus; }

		float GetZFar_RT() { return m_values_RT.zfar; }

		float GetZNear_RT(){ return m_values_RT.znear; }

		float GetFov_RT(){ return m_values_RT.fov; }

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_CAMERA; }
	};
}