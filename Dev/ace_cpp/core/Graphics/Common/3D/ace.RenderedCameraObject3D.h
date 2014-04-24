
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

		} m_values_FR;

		RenderTexture2D_Imp*	m_renderTargetNormalDepth_FR;
		RenderTexture2D_Imp*	m_renderTargetSSAO_FR;
		RenderTexture2D_Imp*	m_renderTargetSSAO_temp_FR;

		RenderTexture2D_Imp*	m_renderTarget_FR[2];
		DepthBuffer_Imp*	m_depthBuffer_FR;

		std::vector<std::shared_ptr<Material2DCommand>>	m_postEffectCommands;
		std::vector<std::shared_ptr<Material2DCommand>>	m_postEffectCommands_FR;

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

		void CalculateMatrix_FR() override;

		void ApplyPostEffects_FR();

		RenderTexture2D_Imp* GetRenderTarget_FR();
		RenderTexture2D_Imp* GetAffectedRenderTarget_FR();

		DepthBuffer_Imp* GetDepthBuffer_FR() { return m_depthBuffer_FR; }

		RenderTexture2D_Imp*	GetRenderTargetDepth_FR() { return m_renderTargetNormalDepth_FR; }

		RenderTexture2D_Imp*	GetRenderTargetSSAO_FR() { return m_renderTargetSSAO_FR; }
		RenderTexture2D_Imp*	GetRenderTargetSSAO_Temp_FR() { return m_renderTargetSSAO_temp_FR; }

		const Matrix44& GetCameraMatrix_FR() { return m_values_FR.cameraMatrix; }
		const Matrix44& GetProjectionMatrix_FR() { return m_values_FR.projectionMatrix; }

		Vector3DF GetFocus_FR(){ return m_values_FR.focus; }

		float GetZFar_FR() { return m_values_FR.zfar; }

		float GetZNear_FR(){ return m_values_FR.znear; }

		float GetFov_FR(){ return m_values_FR.fov; }

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_CAMERA; }
	};
}