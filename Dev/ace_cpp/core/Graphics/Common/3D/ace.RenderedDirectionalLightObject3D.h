
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	/**
		@brief	無限遠から注ぐ光源オブジェクトのクラス
		@note
		ローカル座標の(0,0,1)の方向に光を照射する。
	*/
	class RenderedDirectionalLightObject3D
		: public RenderedObject3D
	{
	private:

		const int32_t ShadowBufferSize = 2048;

#pragma region RenderingThread
		RenderTexture2D_Imp*	m_shadowTexture = nullptr;
		DepthBuffer_Imp*	m_shadowDepthBuffer = nullptr;
		std::vector<Vector3DF>	m_shadowObjectPoints;
		std::vector<Vector3DF>	m_shadowObjectPointsBack;

#pragma endregion

		struct
		{
			Color		color;
		} m_values;

		struct
		{
			Color		color;

		} m_values_RT;

	public:
		RenderedDirectionalLightObject3D(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		void CalcShadowMatrix(Vector3DF viewPosition, Vector3DF viewDirection, Matrix44 matCameraProj, float zn, float zf, Matrix44& lightView, Matrix44& lightProjection);

		Color GetColor_RT();
		Vector3DF GetDirection_RT();

		RenderTexture2D_Imp* GetShadowTexture_RT() { return m_shadowTexture; }
		DepthBuffer_Imp* GetShadowDepthBuffer_RT()  { return m_shadowDepthBuffer; }

		Color GetColor();
		void SetColor(Color color);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT; }
	};
}