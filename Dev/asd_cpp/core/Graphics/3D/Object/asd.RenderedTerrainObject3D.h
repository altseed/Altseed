
#pragma once

#include "asd.RenderedObject3D.h"

namespace asd
{
	class RenderedTerrainObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	m_shaderDF;
		std::shared_ptr<asd::NativeShader_Imp>	m_shaderDF_ND;
		std::shared_ptr<asd::NativeShader_Imp>	m_shaderLightweight;

		std::vector<ShaderConstantValue> shaderConstants;

		std::vector<RenderedObject3DCullingProxy>	cullingProxies;
		std::vector<Culling3D::Object*>				cullingObjects;

		bool	isCullingRegistered = false;
		Renderer3DProxy*	renderer = nullptr;

	public:
		RenderedTerrainObject3DProxy(Graphics* graphics);
		virtual ~RenderedTerrainObject3DProxy();

		Terrain3D* TerrainPtr = nullptr;
			
		std::shared_ptr<MaterialPropertyBlock>	materialPropertyBlock;

		void RegisterCulling();

		void UnregisterCulling();

		void UpdateCullingMatrix(const Matrix44& mat);

		void OnAdded(Renderer3DProxy* renderer) override;

		void OnRemoving(Renderer3DProxy* renderer) override;

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;

		void Rendering(int32_t index, RenderingCommandHelper* helper, RenderingProperty& prop);

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::Terrain; }
	};

	/**
	@brief	地形のクラス
	*/
	class RenderedTerrainObject3D
		: public RenderedObject3D
	{
		friend class RenderedTerrainObject3DProxy;

	private:
		RenderedTerrainObject3DProxy* proxy = nullptr;

		Terrain3D*	terrain = nullptr;

		std::shared_ptr<MaterialPropertyBlock>	materialPropertyBlock;

	public:
		RenderedTerrainObject3D(Graphics* graphics);
		virtual ~RenderedTerrainObject3D();

		void SetMaterialPropertyBlock(MaterialPropertyBlock* block);

		void SetTerrain(Terrain3D* terrain);

		void OnApplyingNextSRT() override;

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::Terrain; }
	};
}