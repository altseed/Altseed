
#pragma once

#include "../../ace.Core.Base.h"

#include "ace.CoreLayer3D.h"

#include "../ace.CoreLayer_Imp.h"
#include "../../Graphics/3D/Renderer/ace.Renderer3D.h"
#include "../../Graphics/2D/ace.LayerRenderer.h"

#include <Math/ace.Vector2DI.h>

namespace ace
{
	class CoreScene;

	class CoreLayer3D_Imp
		: public CoreLayer3D
		, public CoreLayer_Imp
		, public ReferenceObject
	{
	friend class ObjectSystemFactory_Imp;

	private:

		std::set<ObjectPtr> m_objects;

		Renderer3D*		m_renderer;
		
		Core*			core = nullptr;

		bool			m_isDrawnTemp;

		CoreLayer3D_Imp(Core* core, Graphics* graphics, Log* log, Vector2DI windowSize, RenderSettings settings);
		virtual ~CoreLayer3D_Imp();

	public:

		RenderSettings GetRenderSettings() const;
		void SetRenderSettings(RenderSettings settings);

		void AddObject(ObjectPtr object3D);
		void RemoveObject(ObjectPtr object3D);

		float GetAmbientColorIntensity() override;
		void SetAmbientColorIntensity(float ambientColorIntensity) override;

		void SetSkyAmbientColor(Color color) override;
		void SetGroundAmbientColor(Color color) override;

		void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity) override;

		void SetEnvironmentColor(CubemapTexture* diffuseColor, CubemapTexture* specularColor) override;

		float GetSSAO_Radius() override;
		void SetSSAO_Radius(float value) override;
		float GetSSAO_Bias()  override;
		void SetSSAO_Bias(float value) override;
		float GetSSAO_Intensity() override;
		void SetSSAO_Intensity(float value)  override;
		float GetSSAO_FarPlain()  override;
		void SetSSAO_FarPlain(float value) override;

		void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D* texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest) override;

		void BeginUpdating(bool isUpdated) override;
		void EndUpdating(bool isUpdated) override;

		void BeginDrawing();
		
		void Draw();

		void EndDrawing();

		Renderer3D* GetRenderer() { return m_renderer; }

		// 共通部分
		virtual int GetDrawingPriority() const { return CoreLayer_Imp::GetDrawingPriority(); }
		virtual void SetDrawingPriority(int value) { CoreLayer_Imp::SetDrawingPriority(value); }

		virtual bool GetIsDrawn() const { return CoreLayer_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreLayer_Imp::SetIsDrawn(value); }

		virtual void AddPostEffect(CorePostEffect* postEffect) { CoreLayer_Imp::AddPostEffect(postEffect); }
		virtual void ClearPostEffects() { CoreLayer_Imp::ClearPostEffects(); }

		void BeginMeasureUpdateTime() { CoreLayer_Imp::BeginMeasureUpdateTime(); }
		void EndMeasureUpdateTime() { CoreLayer_Imp::EndMeasureUpdateTime(); }
		int GetTimeForUpdate() const { return CoreLayer_Imp::GetTimeForUpdate(); }

#if !SWIG
	public:
		virtual void SetScene(CoreScene* scene) { CoreLayer_Imp::SetScene(scene); }

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}