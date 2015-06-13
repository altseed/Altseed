
#pragma once

#include "asd.RenderedObject3D.h"

#define __CASCATED_SHADOW__	1

namespace asd
{
	class RenderedDirectionalLightObject3DProxy
		: public RenderedObject3DProxy
	{
	private:

#if defined(__CASCATED_SHADOW__)
		RenderTexture2D_Imp*	m_shadowTextures[3];
		DepthBuffer_Imp*		m_shadowDepthBuffers[3];
#else
		RenderTexture2D_Imp*	m_shadowTexture = nullptr;
		DepthBuffer_Imp*		m_shadowDepthBuffer = nullptr;
#endif

		std::vector<Vector3DF>	m_shadowObjectPoints;
		std::vector<Vector3DF>	m_shadowObjectPointsBack;
		GraphicsDeviceType		deviceType;

		/**
			@brief	点群の各要素の最大と最小を計算する。
		*/
		void CalcAABB(std::vector<Vector3DF>& points, Vector3DF& max_, Vector3DF& min_);
		
		/**
			@brief	視錐体を計算する。
			@param	matCameraProj	カメラプロジェクション行列
			@param	near_		視錐体のうち、どの領域を使用するか、の近距離面(0.0～1.0)
			@param	far_		視錐体のうち、どの領域を使用するか、の遠距離面(0.0～1.0)
			@param	deviceType	デバイスの種類
			@param	points	出力先
		*/
		void CalcFrustum(const Matrix44& matCameraProj, float near_, float far_, GraphicsDeviceType deviceType, Vector3DF points[8]);

		/**
			@brief	バンディングボックスを視錐体に収める行列を計算する。
		*/
		Matrix44 CalcClipMatrix(Vector3DF max_, Vector3DF min_, GraphicsDeviceType deviceType);

	public:
		Color		LightColor;
		float		Intensity;

#if defined(__CASCATED_SHADOW__)
		RenderTexture2D_Imp* GetShadowTextures(int32_t index) { return m_shadowTextures[index]; }
		DepthBuffer_Imp* GetShadowDepthBuffers(int32_t index)  { return m_shadowDepthBuffers[index]; }
#else
		RenderTexture2D_Imp* GetShadowTexture() { return m_shadowTexture; }
		DepthBuffer_Imp* GetShadowDepthBuffer()  { return m_shadowDepthBuffer; }
#endif
		

		RenderedDirectionalLightObject3DProxy(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3DProxy();

		Vector3DF GetDirection();

		void CalcShadowMatrix(Vector3DF viewPosition, Vector3DF viewDirection, Vector3DF viewUp, Matrix44 matCameraProj, float zn, float zf, float czn, float czf, Matrix44& lightView, Matrix44& lightProjection);
	
		void CalcUniformShadowMatrix(Vector3DF viewPosition, Vector3DF viewDirection, Vector3DF viewUp, Matrix44 matCameraProj, float zn, float zf, float czn, float czf, Matrix44& lightView, Matrix44& lightProjection);

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::DirectionalLight; }
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
#if defined(__CASCATED_SHADOW__)
		static const int32_t ShadowBufferSize_Large = 2048;
		static const int32_t ShadowBufferSize_Small = 1024;

#else
		static const int32_t ShadowBufferSize = 2048;
#endif

		Color		color;
		float		intensity = 1.0f;
		RenderedDirectionalLightObject3DProxy* proxy = nullptr;

	public:
		RenderedDirectionalLightObject3D(Graphics* graphics);
		virtual ~RenderedDirectionalLightObject3D();

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		float GetIntensity();
		void SetIntensity(float intensity);

		Color GetColor();
		void SetColor(Color color);

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::DirectionalLight; }
	};
}