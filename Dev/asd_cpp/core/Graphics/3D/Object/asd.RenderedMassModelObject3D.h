
#pragma once

#include "asd.RenderedObject3D.h"
#include <Graphics/asd.Graphics.Common.h>
#include <Math/asd.Vector4DF.h>

namespace asd
{
	class RenderedMassModelObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		GraphicsDeviceType		deviceType;
		
		std::shared_ptr<asd::NativeShader_Imp>	m_shadersDF[2];
		std::shared_ptr<asd::NativeShader_Imp>	m_shadersDF_ND[2];
		std::shared_ptr<asd::NativeShader_Imp>	m_shadersLightweight[2];

		std::vector<ShaderConstantValue> shaderConstants;

		RenderedObject3DCullingProxy	cullingProxy;

		static const int32_t MaxObject = 32;

	public:
		Culling3D::Object*						CullingObject = nullptr;

		RenderedMassModelObject3DProxy(Graphics* graphics);
		virtual ~RenderedMassModelObject3DProxy();

		MassModel*				ModelPtr = nullptr;
		
		int32_t					AnimationIndex0 = 0;
		int32_t					AnimationIndex1 = 0;
		float					AnimationTime0 = 0.0;
		float					AnimationTime1 = 0.0;
		float					AnimationWeight = 0.0f;

		std::shared_ptr<MaterialPropertyBlock>	materialPropertyBlock;

		void OnAdded(Renderer3DProxy* renderer) override;
		void OnRemoving(Renderer3DProxy* renderer) override;

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;
		void Draw(RenderingCommandHelper* helper, RenderingProperty& prop, std::vector<RenderedMassModelObject3DProxy*>& proxies, int32_t offset, int32_t count);

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::MassObject; }
	};

	class RenderedMassModelObject3D
		: public RenderedObject3D
	{
		friend class RenderedMassModelObject3DProxy;

	private:

		RenderedMassModelObject3DProxy* proxy = nullptr;

		MassModel*						model = nullptr;

		int32_t					animationIndex0 = 0;
		int32_t					animationIndex1 = 0;
		float					animationTime0 = 0.0;
		float					animationTime1 = 0.0;
		float					animationWeight = 0.0f;
		bool					isAnimationPlaying0 = false;
		bool					isAnimationPlaying1 = false;

		bool					isFading = false;
		float					variation = 0.0f;

		std::shared_ptr<MaterialPropertyBlock>	materialPropertyBlock;

	public:
		RenderedMassModelObject3D(Graphics* graphics);
		virtual ~RenderedMassModelObject3D();

		void SetMaterialPropertyBlock(MaterialPropertyBlock* block);

		MassModel* GetModel();
		void SetModel(MassModel* model);

		void PlayAnimation(const achar* name);

		void StopAnimation();

		void CrossFadeAnimation(const achar* name, float time);

		bool GetIsAnimationPlaying();

		void OnApplyingNextSRT() override;

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::MassObject; }
	};
}