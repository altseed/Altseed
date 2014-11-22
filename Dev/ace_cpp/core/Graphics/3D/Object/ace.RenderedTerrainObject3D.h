
#pragma once

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedTerrainObject3DProxy
		: public RenderedObject3DProxy
	{
	private:
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderDF_ND;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderLightweight;

		std::shared_ptr<ace::NativeShader_Imp>	m_shaderBlackLightweight;
		std::shared_ptr<ace::NativeShader_Imp>	m_shaderBlack;

		std::vector<ShaderConstantValue> shaderConstants;

	public:
		RenderedTerrainObject3DProxy(Graphics* graphics);
		virtual ~RenderedTerrainObject3DProxy();

		Terrain3D* TerrainPtr = nullptr;
			
		std::shared_ptr<MaterialPropertyBlock>	materialPropertyBlock;

		void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) override;
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

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_TERRAIN; }
	};
}