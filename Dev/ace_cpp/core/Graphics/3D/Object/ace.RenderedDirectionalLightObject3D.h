
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedDirectionalLightObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		RenderTexture2D_Imp*	m_shadowTexture = nullptr;
		DepthBuffer_Imp*		m_shadowDepthBuffer = nullptr;
		std::vector<Vector3DF>	m_shadowObjectPoints;
		std::vector<Vector3DF>	m_shadowObjectPointsBack;
		GraphicsDeviceType		deviceType;

		/**
			@brief	点群の各要素の最大と最小を計算する。
		*/
		void CalcAABB(std::vector<Vector3DF>& points, Vector3DF& max_, Vector3DF& min_);

	public:
		Color		LightColor;

		RenderTexture2D_Imp* GetShadowTexture() { return m_shadowTexture; }
		DepthBuffer_Imp* GetShadowDepthBuffer()  { return m_shadowDepthBuffer; }

		RenderedDirectionalLightObject3DProxy(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3DProxy();

		Vector3DF GetDirection();

		void CalcShadowMatrix(Vector3DF viewPosition, Vector3DF viewDirection, Vector3DF viewUp, Matrix44 matCameraProj, float zn, float zf, Matrix44& lightView, Matrix44& lightProjection);
	};

	/**
		@brief	無限遠から注ぐ光源オブジェクトのクラス
		@note
		ローカル座標の(0,0,1)の方向に光を照射する。
	*/
	class RenderedDirectionalLightObject3D
		: public RenderedObject3D
	{
		friend class RenderedDirectionalLightObject3DProxy;

	private:

		static const int32_t ShadowBufferSize = 2048;

		Color		color;
		RenderedDirectionalLightObject3DProxy* proxy = nullptr;

	public:
		RenderedDirectionalLightObject3D(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3D();

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		Color GetColor();
		void SetColor(Color color);

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT; }
	};
}