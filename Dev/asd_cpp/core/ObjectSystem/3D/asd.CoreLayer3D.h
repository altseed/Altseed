
#pragma once

#include "../asd.CoreLayer.h"

namespace asd
{
	class CoreLayer3D
		: public CoreLayer
	{
	protected:

		/**
		@brief	コンストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		CoreLayer3D(){}

		/**
		@brief	デストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		virtual ~CoreLayer3D(){}

	public:
		typedef CoreObject3D* ObjectPtr;

		virtual RenderSettings GetRenderSettings() const = 0;
		virtual void SetRenderSettings(RenderSettings settings) = 0;

		virtual void AddObject(ObjectPtr object3D) = 0;
		virtual void RemoveObject(ObjectPtr object3D) = 0;

		virtual float GetAmbientColorIntensity() = 0;
		virtual void SetAmbientColorIntensity(float ambientColorIntensity) = 0;

		virtual void SetSkyAmbientColor(Color color) = 0;
		virtual void SetGroundAmbientColor(Color color) = 0;

		virtual void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity) = 0;

		virtual void SetEnvironmentColor(CubemapTexture* diffuseColor, CubemapTexture* specularColor) = 0;

		virtual float GetSSAO_Radius() = 0;
		virtual void SetSSAO_Radius(float value) = 0;
		virtual float GetSSAO_Bias() = 0;
		virtual void SetSSAO_Bias(float value) = 0;
		virtual float GetSSAO_Intensity() = 0;
		virtual void SetSSAO_Intensity(float value) = 0;
		virtual float GetSSAO_FarPlain() = 0;
		virtual void SetSSAO_FarPlain(float value) = 0;

		virtual bool GetIsHeightFogEnabled() = 0;
		virtual void SetIsHeightFogEnabled(bool value) = 0;
		virtual float GetHeightFogDensity() = 0;
		virtual void SetHeightFogDensity(float value) = 0;
		virtual Color GetHeightFogColor() = 0;
		virtual void SetHeightFogColor(Color value) = 0;
		virtual float GetHeightFogFalloff() = 0;
		virtual void SetHeightFogFalloff(float value) = 0;
		virtual float GetHeightFogStartDistance() = 0;
		virtual void SetHeightFogStartDistance(float value) = 0;

		virtual bool GetIsOceanEnabled() = 0;
		virtual void SetIsOceanEnabled(bool value) = 0;
		virtual RectF GetOceanArea() = 0;
		virtual void SetOceanArea(RectF value) = 0;
		virtual float GetOceanHeight() = 0;
		virtual void SetOceanHeight(float value) = 0;
		virtual float GetOceanGridSize() = 0;
		virtual void SetOceanGridSize(float value) = 0;
		virtual Texture2D* GetOceanNormalMap() = 0;
		virtual void SetOceanNormalMap(Texture2D* value) = 0;

		virtual void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D* texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest) = 0;
	};
}